/*-
 * Copyright (c) 2007-2015 Matt Macy <mmacy@nextbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");


#include <sys/param.h>
#include <sys/systm.h>
#include <sys/counter.h>
#include <sys/kernel.h>
#include <sys/libkern.h>
#include <sys/malloc.h>
#include <sys/ktr.h>
#include <sys/pcpu.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/sysctl.h>

#include <sys/buf_ring.h>
#include <sys/buf_ring_sc.h>

#define ESTALLED        254      /* consumer is stalled */
#define EOWNED          255      /* consumer lock acquired */

#define ALIGN_SCALE (CACHE_LINE_SIZE/sizeof(caddr_t))


static struct buf_ring *
buf_ring_alloc_(int count, struct malloc_type *type, int flags, struct mtx *lock, int brflags)
{
	struct buf_ring *br;
	int alloc_count;

	KASSERT(powerof2(count), ("buf ring must be size power of 2"));
	alloc_count = (brflags & BR_FLAGS_ALIGNED) ? (count * ALIGN_SCALE) : count; 

	br = malloc(sizeof(struct buf_ring) + alloc_count*sizeof(caddr_t),
	    type, flags|M_ZERO);
	if (br == NULL)
		return (NULL);
	br->br_flags = brflags;
#ifdef DEBUG_BUFRING
	br->br_lock = lock;
#endif	
	br->br_prod_size = br->br_cons_size = count;
	br->br_prod_mask = br->br_cons_mask = count-1;
	br->br_prod_head = br->br_cons_head = 0;
	br->br_prod_tail = br->br_cons_tail = 0;

	return (br);
}

struct buf_ring *
buf_ring_alloc(int count, struct malloc_type *type, int flags, struct mtx *lock)
{

	return (buf_ring_alloc_(count, type, flags, lock, 0));
}

struct buf_ring *
buf_ring_aligned_alloc(int count, struct malloc_type *type, int flags, struct mtx *lock)
{

	return (buf_ring_alloc_(count, type, flags, lock, BR_FLAGS_ALIGNED));
}

void
buf_ring_free(struct buf_ring *br, struct malloc_type *type)
{

	free(br, type);
}

/*
 * buf_ring_sc definitions follow
 */

#ifndef BRSC_DEBUG_COUNTERS
#ifdef INVARIANTS
#define BRSC_DEBUG_COUNTERS 1
#else
#define BRSC_DEBUG_COUNTERS 0
#endif /* !INVARIANTS */
#endif

#if BRSC_DEBUG_COUNTERS
static SYSCTL_NODE(_net, OID_AUTO, brsc, CTLFLAG_RD, 0,
                   "buf_ring_stats");

static int brsc_drains;
static int brsc_drain_handled;
static int brsc_nqs;
static int brsc_nq_pendings;
static int brsc_nq_entry_sets;
static int brsc_nq_entry_putbacks;
static int brsc_nq_entry_set_nulls;
static int brsc_nq_entry_gets;
static int brsc_nq_entry_advances;

static int brsc_nq_enobufs1;
static int brsc_nq_enobufs2;
static int brsc_nq_eowned;
static int brsc_nq_domain_eq;
static int brsc_deferred;


SYSCTL_INT(_net_brsc, OID_AUTO, drain, CTLFLAG_RD,
		   &brsc_drains, 0, "# times drain was called");
SYSCTL_INT(_net_brsc, OID_AUTO, drain_handled, CTLFLAG_RD,
		   &brsc_drain_handled, 0, "# times drain handler was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nqs, CTLFLAG_RD,
		   &brsc_nqs, 0, "# times enqueue was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_pendings, CTLFLAG_RD,
		   &brsc_nq_pendings, 0, "# times pending was set in nq");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_entry_sets, CTLFLAG_RD,
		   &brsc_nq_entry_sets, 0, "# times entry_set was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_entry_gets, CTLFLAG_RD,
		   &brsc_nq_entry_gets, 0, "# times entry_get was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_entry_putbacks, CTLFLAG_RD,
		   &brsc_nq_entry_putbacks, 0, "# times entry_putback was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_entry_advances, CTLFLAG_RD,
		   &brsc_nq_entry_advances, 0, "# times entry_advance was called");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_entry_set_nulls, CTLFLAG_RD,
		   &brsc_nq_entry_set_nulls, 0, "# times entry_set was called with null");
SYSCTL_INT(_net_brsc, OID_AUTO, enobufs1, CTLFLAG_RD,
		   &brsc_nq_enobufs1, 0, "# times ENOBUFS1 was returned from nq");
SYSCTL_INT(_net_brsc, OID_AUTO, enobufs2, CTLFLAG_RD,
		   &brsc_nq_enobufs2, 0, "# times ENOBUFS2 was returned from nq");
SYSCTL_INT(_net_brsc, OID_AUTO, eowned, CTLFLAG_RD,
		   &brsc_nq_eowned, 0, "# times lock was acquired in enqueue");
SYSCTL_INT(_net_brsc, OID_AUTO, nq_domain_eq, CTLFLAG_RD,
		   &brsc_nq_domain_eq, 0, "# times domain was set and matched");
SYSCTL_INT(_net_brsc, OID_AUTO, deferred, CTLFLAG_RD,
		   &brsc_deferred, 0, "# times deferred");


#define DBG_COUNTER_INC(name) atomic_add_int(&(brsc_ ## name), 1)
#else
#define DBG_COUNTER_INC(name)
#endif

struct br_sc_entry_ {
	volatile void *bre_ptr;
};


#define BR_RING_ABDICATING (1<<31)
#define BR_RING_STALLED    (1<<30)
#define BR_RING_IDLE       (1<<29)
#define BR_RING_MAX        (1<<28)
#define BR_RING_MASK       (BR_RING_MAX-1)
#define BR_RING_FLAGS_MASK       (~(BR_RING_MASK))

#define BR_INDEX(br, x) ((x) & br->br_mask)
#define BR_HANDOFF(br) ((br)->br_cons & (BR_RING_ABDICATING|BR_RING_IDLE))
#define BR_STALLED(br) ((br)->br_cons & BR_RING_STALLED)
#define BR_CONS_IDX(br) ((br)->br_cons & br->br_mask)


#define BR_RING_GEN		(1<<29)
#define BR_RING_PENDING (1<<30)
#define BR_RING_OWNED   (1<<31)

#define BR_GEN(br) (br->br_prod_head & BR_RING_GEN)

#define BR_NODOMAIN 0xffffffff

typedef enum br_state_ {
	BR_IDLE = 1,
	BR_ABDICATED,
	BR_BUSY,
	BR_STALLED
} br_state;

struct buf_ring_sc {
	volatile uint32_t	br_prod_head;
	volatile uint32_t	br_prod_tail;
	/*
	 * The following values are not expected to be modified
	 * while the ring is in use, so we put them on their own
	 * cache line to avoid false sharing when they are read
	 */
	int              	br_size __aligned(CACHE_LINE_SIZE);
	int              	br_mask;
	int			br_flags;
	int			br_domain;
	counter_u64_t		br_enqueues;
	counter_u64_t		br_drops;
	counter_u64_t		br_starts;
	counter_u64_t		br_restarts;
	counter_u64_t		br_abdications;
	counter_u64_t		br_stalls;
	int (*br_drain) (struct buf_ring_sc *br, int avail, void *sc);
	void (*br_deferred) (struct buf_ring_sc *br, void *sc);
	void *br_sc;
	struct thread		*br_owner;
	/* cache line aligned to avoid cache line invalidate traffic
	 * between consumer and producer (false sharing)
	 *
	 */
	volatile uint32_t	br_cons __aligned(CACHE_LINE_SIZE);
	/* cache line aligned to avoid false sharing with other data structures
	 * located just beyond the end of the ring
	 */
	struct br_sc_entry_	br_ring[0] __aligned(CACHE_LINE_SIZE);
};

static void buf_ring_sc_lock(struct buf_ring_sc *br);
static int buf_ring_sc_trylock(struct buf_ring_sc *br);
static int buf_ring_sc_unlock(struct buf_ring_sc *br, br_state state);

static void buf_ring_sc_advance(struct buf_ring_sc *br, int count);

/*
 * Many architectures other than x86 permit speculative re-ordering
 * of loads. Unfortunately, atomic_load_acq_32() is comparatively
 * expensive so we'd rather elide it if possible.
 */
#if defined(__i386__) || defined(__amd64__)
#define ORDERED_LOAD_32(x) (*x)
#else
#define ORDERED_LOAD_32(x) atomic_load_acq_32((x))
#endif


static inline int
brsc_get_inuse(struct buf_ring_sc *br, int cidx, int pidx, int gen)
{
	int used;

	if (gen && pidx == cidx)
		used = br->br_size;
	else if (pidx >= cidx)
		used = pidx - cidx; /* encompasses gen == 0 && pidx == cidx */
	else
		used = br->br_size - cidx + pidx;

	return (used);
}

static inline int
brsc_get_avail(struct buf_ring_sc *br, int cidx, int pidx, int gen)
{

	return (br->br_size - brsc_get_inuse(br, cidx, pidx, gen));
}

/*
 * ring entry accessors to allow us to make ring entry
 * alignment determined at runtime
 */
static __inline void *
brsc_entry_get(struct buf_ring_sc *br, int i)
{
	volatile void *ent;

	MPASS(i >= 0 && i < br->br_size);

	if (br->br_flags & BR_FLAGS_ALIGNED)
		ent = br->br_ring[i*ALIGN_SCALE].bre_ptr;
	else
		ent = br->br_ring[i].bre_ptr;
	return ((void *)(uintptr_t)ent);
}

static __inline void
brsc_entry_set(struct buf_ring_sc *br, int i, void *buf)
{
	volatile void *bufp;

	MPASS(i >= 0 && i < br->br_size);

	DBG_COUNTER_INC(nq_entry_sets);

	if (br->br_flags & BR_FLAGS_ALIGNED)
		bufp = (volatile void *)&br->br_ring[i*ALIGN_SCALE].bre_ptr;
	else
		bufp = (volatile void *)&br->br_ring[i].bre_ptr;

	if (buf == NULL) {
		DBG_COUNTER_INC(nq_entry_set_nulls);
		MPASS(*(void * volatile *)bufp != NULL);
	}
	*((void * volatile *)bufp) = buf;
}

static inline void
brsc_gen_clear(struct buf_ring_sc *br)
{

	atomic_clear_acq_int(&br->br_prod_head, BR_RING_GEN);
}

struct buf_ring_sc *
buf_ring_sc_alloc(int count, struct malloc_type *type, int flags,
	struct buf_ring_sc_consumer *brsc)
{
	struct buf_ring_sc *br;
	int alloc_count = count;

	KASSERT(powerof2(count), ("buf ring must be size power of 2"));
	if (brsc->brsc_flags & BR_FLAGS_ALIGNED)
		alloc_count = count*ALIGN_SCALE;
	br = malloc(sizeof(struct buf_ring) + alloc_count*sizeof(caddr_t),
	    type, flags|M_ZERO);
	if (br == NULL)
		return (NULL);

	br->br_drain = brsc->brsc_drain;
	br->br_deferred = brsc->brsc_deferred;
	br->br_flags = brsc->brsc_flags;
	br->br_sc = brsc->brsc_sc;
	if (brsc->brsc_flags & BR_FLAGS_NUMA)
		br->br_domain = brsc->brsc_domain;
	else
		br->br_domain = BR_NODOMAIN;
	br->br_size = count;
	br->br_mask = count-1;
	br->br_prod_head = br->br_prod_tail = 0;
	br->br_cons = 0;
	br->br_enqueues = counter_u64_alloc(flags);
	br->br_drops = counter_u64_alloc(flags);
	br->br_abdications = counter_u64_alloc(flags);
	br->br_stalls = counter_u64_alloc(flags);
	br->br_starts = counter_u64_alloc(flags);
	br->br_restarts = counter_u64_alloc(flags);
	buf_ring_sc_reset_stats(br);
	return (br);
}

void
buf_ring_sc_free(struct buf_ring_sc *br, struct malloc_type *type)
{
	counter_u64_free(br->br_enqueues);
	counter_u64_free(br->br_drops);
	counter_u64_free(br->br_abdications);
	counter_u64_free(br->br_stalls);
	counter_u64_free(br->br_starts);
	counter_u64_free(br->br_restarts);

	free(br, type);
}

void
buf_ring_sc_reset_stats(struct buf_ring_sc *br)
{

	counter_u64_zero(br->br_enqueues);
	counter_u64_zero(br->br_drops);
	counter_u64_zero(br->br_abdications);
	counter_u64_zero(br->br_stalls);
	counter_u64_zero(br->br_starts);
	counter_u64_zero(br->br_restarts);
}

void
buf_ring_sc_get_stats_v0(struct buf_ring_sc *br, struct buf_ring_sc_stats_v0 *brss)
{
	brss->brs_enqueues = counter_u64_fetch(br->br_enqueues);
	brss->brs_drops = counter_u64_fetch(br->br_drops);
	brss->brs_abdications = counter_u64_fetch(br->br_abdications);
	brss->brs_stalls = counter_u64_fetch(br->br_stalls);
	brss->brs_starts = counter_u64_fetch(br->br_starts);
	brss->brs_restarts = counter_u64_fetch(br->br_restarts);
}

static br_state
buf_ring_sc_drain_locked(struct buf_ring_sc *br, int budget)
{
	uint32_t cidx = BR_CONS_IDX(br);
	uint32_t pidx = br->br_prod_tail;
	uint32_t gen = BR_GEN(br);
	uint32_t n;
	int inuse, prod_avail;
	br_state state;

	DBG_COUNTER_INC(drains);

	KASSERT(budget > 0, ("calling drain with invalid budget"));
	MPASS(br->br_prod_head & BR_RING_OWNED);
	MPASS(br->br_owner == curthread);
	state = BR_IDLE;
	while (budget && (cidx != pidx || (gen && (pidx == cidx)))) {
		inuse = brsc_get_inuse(br, cidx, pidx, gen);
		prod_avail = min(inuse, budget);
		MPASS(prod_avail > 0);
		DBG_COUNTER_INC(drain_handled);
		MPASS(br->br_owner == curthread);
		n = br->br_drain(br, prod_avail, br->br_sc);
		MPASS(br->br_owner == curthread);
		KASSERT(n <= prod_avail, ("drain handler return invalid count"));
		if (n == 0) {
			state = BR_STALLED;
			break;
		}

		buf_ring_sc_advance(br, n);
		budget -= n;
		if (budget == 0) {
			if (BR_CONS_IDX(br) != br->br_prod_tail)
				state = BR_ABDICATED;
			else
				state = BR_IDLE;
			break;
		}
		gen = BR_GEN(br);
		pidx = br->br_prod_tail;
		cidx = BR_CONS_IDX(br);
	}

	return (state);
}

void
buf_ring_sc_drain(struct buf_ring_sc *br, int budget)
{
	br_state state;
	int pending;

	KASSERT(budget >= 0, ("calling drain with invalid budget"));
	critical_enter();
	if (br->br_domain == PCPU_GET(domain))
		sched_pin();
	else if (br->br_domain != BR_NODOMAIN) {
		br->br_deferred(br, br->br_sc);
		critical_exit();
		return;
	}
	critical_exit();

	if (__predict_false(budget == 0)) {
		buf_ring_sc_lock(br);
		budget = br->br_size;
	} else if (!buf_ring_sc_trylock(br)) {
		if (br->br_domain == PCPU_GET(domain))
			sched_unpin();
		return;
	}

	state = buf_ring_sc_drain_locked(br, budget);
	if (br->br_domain == PCPU_GET(domain))
			sched_unpin();
	pending = buf_ring_sc_unlock(br, state);
	if ((state == BR_ABDICATED) && pending == 0)
		br->br_deferred(br, br->br_sc);
}

/*
 * Multi-producer safe lock-free ring buffer enqueue
 *
 * Most architectures do not support the atomic update of multiple
 * discontiguous locations. So it is not possible to atomically update
 * the producer index and ring buffer entry. To side-step this limitation
 * we split update in to 3 steps:
 *      1) atomically acquiring an index
 *      2) updating the corresponding ring entry
 *      3) making the update available to the consumer
 * In order to split the index update in to an acquire and release
 * phase there are _two_ producer indexes. 'prod_head' is used for
 * step 1) and is thus only used by the enqueue itself. 'prod_tail'
 * is used for step 3) to signal to the consumer that the update is
 * complete. To guarantee memory ordering the update of 'prod_tail' is
 * done with a atomic_store_rel_32(...) and the corresponding
 * initial read of 'prod_tail' by the dequeue functions is done with
 * an atomic_load_acq_32(...).
 *
 * Regarding memory ordering - there are five variables in question:
 * (br_) prod_head, prod_tail, cons, ring[idx={cons, prod}]
 * It's easiest examine correctness by considering the consequence of
 * reading a stale value or having an update become visible prior to
 * preceding writes.
 *
 * - prod_head: this is only read by the enqueue routine, if the latter were to
 *   initially read a stale value for it the cmpxchg (atomic_cmpset_acq_32)
 *   would fail. However, the implied memory barrier in cmpxchg would cause the
 *   subsequent read of prod_head to read the up-to-date value permitting the
 *   cmpxchg to succeed the second time.
 *
 * - prod_tail: This value is used by dequeue to determine the effective
 *   producer index. On architectures with weaker memory ordering than x86 it
 *   needs special handling. In enqueue it needs to be updated with
 *   atomic_store_rel_32() (i.e. a write memory barrier before update) to
 *   guarantee that the new ring value is committed to memory before it is
 *   made available by prod_tail. In dequeue to guarantee that it is read before
 *   br_ring[cons] it needs to be read with atomic_load_acq_32().
 *
 *
 * - cons: This is used to communicate the latest consumer index between
 *   dequeue and enqueue. Reading a stale value in enqueue can cause an enqueue
 *   to fail erroneously. To avoid a load being re-ordered after a store (and
 *   thus permitting enqueue to store a new value before the old one has been
 *   consumed) it is updated with an atomic_store_rel_32() in deqeueue.
 *
 * - ring[idx] : Updates to this value need to reach memory before the subsequent
 *   update to prod_tail does. Reads need to happen before subsequent updates to
 *   cons.
 *
 * Some implementation notes:
 * - Much like a simpler single-producer single consumer ring buffer,
 *   the producer can not produce faster than the consumer. Hence the
 *   check of 'prod_head' + 1 against 'cons'.
 *
 * - The use of "prod_next = (prod_head + 1) & br->br_mask" to
 *   calculate the next index is slightly cheaper than a modulo but
 *   requires the ring to be power-of-2 sized.
 *
 * - The critical_enter() / critical_exit() are not required for
 *   correctness. They prevent updates from stalling by having a producer be
 *   preempted after updating 'prod_head' but before updating 'prod_tail'.
 *
 * - The "while (br->br_prod_tail != prod_head)"
 *   check assures in order completion allows us to update
 *   'prod_tail' without a cmpxchg / LOCK prefix assures in order
 *   completion as a later producer might reach this point before an
 *   earlier consumer.
 *
 *
 *   This buf_ring has the following FSM:
 *     producer: 
 *     -  !owned              -> owned(curthread) + enqueue
 *     -  pending(!curthread) -> enqueue 
 *     -  owned + abdicating  -> owned + abdicating + pending(curthread)
 *     -  owned + abdicating + pending(curthread) -> 
 *            owned + abdicating + pending(curthread) + enqueue
 *     -  owned + abdicating + pending(curthread) + enqueue  ->
 *           wait(!owned)
 *     - !owned + abdicating + pending(curthread) ->
 *        owned + busy + pending(curthread) -> 
 *        owned + busy (consumer) 
 *     consumer (i.e. owned(curthread)):
 *      -  busy + owned -> abdicating + owned
 *      -  abdicating + owned + pending -> abdicating + unowned + pending
 *      -  abdicating + owned -> abdicating + unowned + enqueue tx task
 *      How do we handle abdication when the ring is full
 */

int
buf_ring_sc_enqueue(struct buf_ring_sc *br, void *ents[], int count, int budget)
{
	uint32_t prod_head, prod_next, cons;
	uint32_t pidx, cidx, pidx_next;
	int i, pending, rc, avail, domainvalid;
#ifdef DEBUG_BUFRING
	int j;
	for (i = BR_CONS_IDX(br); i != ORDERED_LOAD_32(&br->br_prod_tail);
	     i = ((i + 1) & br->br_mask))
		for (j = 0; j < count; j++)
			if (brsc_entry_get(br, i) == ents[j])
				panic("buf=%p already enqueue at %d prod=%d cons=%d",
					  ents[j], i, br->br_prod_tail, BR_CONS_IDX(br));
#endif
	MPASS(count > 0);
	DBG_COUNTER_INC(nqs);
	critical_enter();

	if (br->br_domain == BR_NODOMAIN || br->br_domain == PCPU_GET(domain))
		domainvalid = 1;
	prod_head = br->br_prod_head;
	pending = false;
	rc = 0;

	/*
	 * If the current consumer abdicated and no one has set the pending bit yet
	 * we loop until we set it we're the next lock holder - or if the owner
	 * drops the lock before we can do that then the lock will be
	 * re-acquired normally
	 */
	while (BR_HANDOFF(br) && (prod_head & BR_RING_FLAGS_MASK) == BR_RING_OWNED && budget > 0 && domainvalid) {
		prod_head = br->br_prod_head;
		pidx = BR_INDEX(br, prod_head);
		cons = br->br_cons;
		cidx = BR_INDEX(br, cons);
		avail = brsc_get_avail(br, cidx, pidx, BR_GEN(br));

		if (count > avail) {
			if (pidx != BR_INDEX(br, atomic_load_acq_32(&br->br_prod_head)) ||
			    cidx != BR_INDEX(br, atomic_load_acq_32(&br->br_cons)))
				continue;
			critical_exit();
			counter_u64_add(br->br_drops, 1);
			DBG_COUNTER_INC(nq_enobufs1);
			return (ENOBUFS);
		}

		pidx_next = BR_INDEX(br, pidx + count);
		/* set the pending bit and preserve the owned bit */
		prod_next = pidx_next | BR_RING_PENDING | (prod_head & BR_RING_FLAGS_MASK);
		/* we wrapped set the gen bit to indicate that cidx==pidx => ring full */
		if (pidx_next < pidx)
			prod_next |= BR_RING_GEN;
		if (atomic_cmpset_acq_32(&br->br_prod_head, prod_head, prod_next)) {
			pending = true;
			DBG_COUNTER_INC(nq_pendings);
			goto done;
		}
		prod_head = br->br_prod_head;
	}
	do {
		rc = 0;
		prod_head = br->br_prod_head;
		pidx = BR_INDEX(br, prod_head);
		cidx = BR_INDEX(br, br->br_cons);
		avail = brsc_get_avail(br, cidx, pidx, BR_GEN(br));

		if (count > avail) {
			/* ensure that we only return ENOBUFS
			 * if the latest value matches what we read
			 */
			if (pidx != BR_INDEX(br, atomic_load_acq_32(&br->br_prod_head)) ||
			    cidx != BR_INDEX(br, atomic_load_acq_32(&br->br_cons)))
				continue;
			critical_exit();
			counter_u64_add(br->br_drops, 1);
			DBG_COUNTER_INC(nq_enobufs2);
			return (ENOBUFS);
		}
		prod_next = pidx_next = BR_INDEX(br, pidx + count);
		/* preserve flags */
		prod_next |= (prod_head & BR_RING_FLAGS_MASK);

		/* we wrapped - set the gen bit to indicate that cidx==pidx => ring full */
		if (pidx_next < pidx)
			prod_next |= BR_RING_GEN;
		/*
		 * If the ring is unowned, not pending, the budget is non-zero, and we're
		 * in the right domain, try to acquire the lock.
		 */
		if ((prod_head & BR_RING_FLAGS_MASK) == 0 && budget > 0 && domainvalid) {
			prod_next |= BR_RING_OWNED;
			DBG_COUNTER_INC(nq_eowned);
			rc = EOWNED;
		}
	} while (!atomic_cmpset_acq_32(&br->br_prod_head, prod_head, prod_next));
	done:
	for (i = 0; i < count; i++) {
		MPASS(ents[i] != NULL);
		brsc_entry_set(br, (BR_INDEX(br, prod_head)-(count-1))+i, ents[i]);
	}
	/*
	 * If there are other enqueues in progress
	 * that preceded us, we need to wait for them
	 * to complete
	 * re-ordering of reads would not effect correctness
	 */
	while (br->br_prod_tail != BR_INDEX(br, prod_head))
		cpu_spinwait();
	/* ensure  that the ring update reaches memory before the new
	 * value of prod_tail
	 */
	atomic_store_rel_32(&br->br_prod_tail, BR_INDEX(br, prod_next));

	/* now that we've completed the enqueue if we know that we're
	 * the next owner we need to wait for the current owner to clear
	 * the owned bit
	 */
	if (pending) {
		while ((br->br_prod_head & BR_RING_OWNED) == BR_RING_OWNED)
			cpu_spinwait();
		atomic_set_acq_32(&br->br_prod_head, BR_RING_OWNED);
		DBG_COUNTER_INC(nq_eowned);
		rc = EOWNED;
	}
	if (rc == EOWNED) {
		MPASS(br->br_owner == NULL);
		MPASS((br->br_prod_head & BR_RING_OWNED) == BR_RING_OWNED);
		/* clear the flags bits from cons */
		br->br_cons &= ~BR_RING_FLAGS_MASK;
		br->br_owner = curthread;
		if (br->br_domain == PCPU_GET(domain))
			sched_pin();
	}
    /*
	 * we became owner by way of the contested abdicate clear pending
	 */
	if (pending)
		atomic_clear_rel_32(&br->br_prod_head, BR_RING_PENDING);

	critical_exit();
	counter_u64_add(br->br_enqueues, 1);

	/* we're the owner - our buffers were enqueued */
	if (rc == EOWNED) {
		br_state state = buf_ring_sc_drain_locked(br, budget);
		if (br->br_domain == PCPU_GET(domain))
			sched_unpin();
		if (buf_ring_sc_unlock(br, state) == 0 && state == BR_ABDICATED) {
			DBG_COUNTER_INC(deferred);
			br->br_deferred(br, br->br_sc); /* consumer's re-drive mechanism */
		}
	}
#if 0
	else if (BR_STALLED(br)) {
		/* buffer enqueued - but we can't do any work */
		return (ESTALLED);
	}
#endif
	return (0);
}

/*
 * populate ents with up to count values from the ring
 * and return the number of entries
 */
int
buf_ring_sc_peek(struct buf_ring_sc *br, void *ents[], uint16_t count)
{
	uint32_t cons;
	uint32_t prod_tail;
	int i, inuse, prod_avail;

	KASSERT(count > 0, ("peeking for zero entries"));
	KASSERT((br->br_prod_head & BR_RING_OWNED) == BR_RING_OWNED, ("peeking without lock being held"));
	MPASS(br->br_owner == curthread);
	/*
	 * for correctness prod_tail must be read before ring[cons]
	 */
	cons = BR_CONS_IDX(br);
	prod_tail = ORDERED_LOAD_32(&br->br_prod_tail);
	if ((inuse = brsc_get_inuse(br, cons, prod_tail, BR_GEN(br))) == 0)
		return (0);

	prod_avail = min(inuse, count);
	for (i = 0; i < prod_avail; i++) {
		DBG_COUNTER_INC(nq_entry_gets);
		ents[i] = brsc_entry_get(br, BR_INDEX(br, cons + i));
		MPASS(ents[i] != NULL);
	}
	return (prod_avail);
}

/*
 * Used to return a buffer (most likely already there)
 * to the top od the ring. The caller should *not*
 * have used any dequeue to pull it out of the ring
 * but instead should have used the peek() function.
 * This is normally used where the transmit queue
 * of a driver is full, and an mubf must be returned.
 * Most likely whats in the ring-buffer is what
 * is being put back (since it was not removed), but
 * sometimes the lower transmit function may have
 * done a pullup or other function that will have
 * changed it. As an optimization we always put it
 * back (since jhb says the store is probably cheaper),
 * if we have to do a multi-queue version we will need
 * the compare and an atomic.
 *
 */
void
buf_ring_sc_putback(struct buf_ring_sc *br, void *new, int idx)
{
	KASSERT(BR_CONS_IDX(br) != br->br_prod_tail,
			("Buf-Ring has none in putback")) ;
	brsc_entry_set(br, BR_CONS_IDX(br) + idx, new);
}

/*
 * @count: the number of entries by which to advance the consumer index
 *
 */
static void
buf_ring_sc_advance(struct buf_ring_sc *br, int count)
{
	uint32_t cons, cons_next;
	uint32_t prod_tail;
	int i;

	KASSERT(count > 0, ("invalid advance count"));
	MPASS(br->br_owner == curthread);

	DBG_COUNTER_INC(nq_entry_advances);

	cons = BR_CONS_IDX(br);
	prod_tail = br->br_prod_tail;
	cons_next = BR_INDEX(br, cons + count);

	/*
	 * Storing NULL here serves two purposes:
	 * 1) it assures that the load of ring[cons] has completed
	 *    (only the most perverted architecture or compiler would
	 *    consider re-ordering a = *x; *x = b)
	 * 2) it allows us to enforce global ordering of the cons
	 *    update with an atomic_store_rel_32
	 */
	for (i = 0; i < count; i++)
		brsc_entry_set(br, BR_INDEX(br, cons + i), NULL);

	if (cons_next < cons) {
		MPASS(cons_next <= prod_tail);
		brsc_gen_clear(br);
	}
	atomic_store_rel_32(&br->br_cons, cons_next);

}

/*
 * mark the ring as being abdicated
 */
void
buf_ring_sc_abdicate(struct buf_ring_sc *br)
{
	uint32_t cons_next;

	cons_next = br->br_cons | BR_RING_ABDICATING;
	counter_u64_add(br->br_abdications, 1);
	critical_enter();

	atomic_store_rel_32(&br->br_cons, cons_next);
}

int
buf_ring_sc_count(struct buf_ring_sc *br)
{
	/*  br_cons and br_prod_tail may be stale but the consumer
	 * understands that this is only a point in time snapshot
	 */

	return (brsc_get_inuse(br, BR_INDEX(br, br->br_cons), br->br_prod_tail, BR_GEN(br)));
}

int
buf_ring_sc_empty(struct buf_ring_sc *br)
{
	/*  br_prod_tail may be stale but the consumer understands that this is
	*  only a point in time snapshot
	*/

	return ((BR_GEN(br) == 0) && BR_INDEX(br, br->br_cons) == br->br_prod_tail);
}

int
buf_ring_sc_full(struct buf_ring_sc *br)
{
	/* br_cons may be stale but the caller understands that this is
	* only a point in time snapshot
	*/
	return (BR_GEN(br) && (BR_INDEX(br, br->br_cons) == br->br_prod_tail));
}

/*
 * currently being used only for flushing all buffers
 */
static void
buf_ring_sc_lock(struct buf_ring_sc *br)
{
	uint32_t value;

	do {
		while ((value = br->br_prod_head) & BR_RING_PENDING)
			cpu_spinwait();
	} while (!atomic_cmpset_acq_32(&br->br_prod_head, value, value | BR_RING_PENDING));
	do {
		while ((value = br->br_prod_head) & BR_RING_OWNED)
			cpu_spinwait();
	} while (!atomic_cmpset_acq_32(&br->br_prod_head, value, value | BR_RING_OWNED));
	br->br_owner = curthread;
	if (br->br_cons & BR_RING_IDLE)
		counter_u64_add(br->br_starts, 1);
	else if (br->br_cons & BR_RING_STALLED)
		counter_u64_add(br->br_restarts, 1);
	br->br_cons &= ~(BR_RING_IDLE|BR_RING_ABDICATING|BR_RING_STALLED);
	atomic_clear_rel_32(&br->br_prod_head, BR_RING_PENDING);
}


static int
buf_ring_sc_trylock(struct buf_ring_sc *br)
{
	uint32_t value;

	do {
		value = br->br_prod_head;
		if (value & (BR_RING_OWNED|BR_RING_PENDING))
			return (0);
	} while (!atomic_cmpset_acq_32(&br->br_prod_head, value, value | BR_RING_OWNED | BR_RING_PENDING));

	MPASS(br->br_owner == NULL);
	MPASS((br->br_prod_head & (BR_RING_OWNED|BR_RING_PENDING)) == (BR_RING_OWNED|BR_RING_PENDING));
	br->br_cons &= ~(BR_RING_IDLE|BR_RING_ABDICATING|BR_RING_STALLED);
	atomic_clear_rel_32(&br->br_prod_head, BR_RING_PENDING);
	br->br_owner = curthread;
	if (br->br_cons & BR_RING_IDLE)
		counter_u64_add(br->br_starts, 1);
	else if (br->br_cons & BR_RING_STALLED)
		counter_u64_add(br->br_restarts, 1);

	return (1);
}

static int
buf_ring_sc_unlock(struct buf_ring_sc *br, br_state reason)
{
	uint32_t prod_head, cons_next;
	int pending;

	KASSERT(br->br_prod_head & BR_RING_OWNED, ("unlocking unowned ring"));
	/*
	 * we treat IDLE the same as ABDICATE to avoid a race
	 * with enqueue - they only differ for purposes of stats
	 * keeping
	 */
	if (reason == BR_IDLE) {
		cons_next = br->br_cons | BR_RING_IDLE;
		critical_enter();
		atomic_store_rel_32(&br->br_cons, cons_next);
	} else if ((reason == BR_ABDICATED) &&
			   (br->br_cons & BR_RING_ABDICATING) == 0) {
		cons_next = br->br_cons | BR_RING_ABDICATING;
		counter_u64_add(br->br_abdications, 1);
		critical_enter();
		atomic_store_rel_32(&br->br_cons, cons_next);
	} else if (reason == BR_STALLED) {
		cons_next = br->br_cons | BR_RING_STALLED;
		counter_u64_add(br->br_stalls, 1);
		critical_enter();
		atomic_store_rel_32(&br->br_cons, cons_next);
	} else
		panic("invalid unlock state");
	br->br_owner = NULL;
	do {
		prod_head = br->br_prod_head;
		pending = !!(prod_head & BR_RING_PENDING);
	} while (!atomic_cmpset_rel_32(&br->br_prod_head, prod_head, prod_head & ~BR_RING_OWNED));
	critical_exit();
	return (pending);
}
