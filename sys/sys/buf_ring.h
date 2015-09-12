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
 *
 * $FreeBSD$
 *
 */

#ifndef	_SYS_BUF_RING_H_
#define	_SYS_BUF_RING_H_

#include <machine/cpu.h>

#if defined(INVARIANTS) && !defined(DEBUG_BUFRING)
#define DEBUG_BUFRING 1
#endif

#ifdef DEBUG_BUFRING
#include <sys/lock.h>
#include <sys/mutex.h>
#endif

/* cache line align buf ring entries */
#define BR_FLAGS_ALIGNED 0x1

struct br_entry_ {
	volatile void *bre_ptr;
};

struct buf_ring {
	volatile uint32_t	br_prod_head;
	volatile uint32_t	br_prod_tail;	
	int              	br_prod_size;
	int              	br_prod_mask;
	uint64_t		br_drops;
	/* cache line aligned to avoid cache line invalidate traffic
	 * between consumer and producer (false sharing)
	 */
	volatile uint32_t	br_cons_head __aligned(CACHE_LINE_SIZE);
	volatile uint32_t	br_cons_tail;
	int		 	br_cons_size;
	int              	br_cons_mask;
#ifdef DEBUG_BUFRING
	struct mtx		*br_lock;
#endif
	/* cache line aligned to avoid false sharing with other data structures
	 */
	int			br_flags  __aligned(CACHE_LINE_SIZE);
	struct br_entry_	br_ring[0] __aligned(CACHE_LINE_SIZE);
};

/*
 * ring entry accessors to allow us to make ring entry
 * alignment determined at runtime
 */
static __inline void *
br_entry_get(struct buf_ring *br, int i)
{
	volatile void *ent;

	if (br->br_flags & BR_FLAGS_ALIGNED)
		ent = br->br_ring[i*(CACHE_LINE_SIZE/sizeof(caddr_t))].bre_ptr;
	else
		ent = br->br_ring[i].bre_ptr;
	return ((void *)(uintptr_t)ent);
}

static __inline void
br_entry_set(struct buf_ring *br, int i, void *buf)
{

	if (br->br_flags & BR_FLAGS_ALIGNED)
		br->br_ring[i*(CACHE_LINE_SIZE/sizeof(caddr_t))].bre_ptr = buf;
	else
		br->br_ring[i].bre_ptr = buf;
}

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
 * (br_) prod_head, prod_tail, cons_head, cons_tail, ring[idx={cons, prod}]
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
 *   br_ring[cons_head] it needs to be read with atomic_load_acq_32().
 *
 * - cons_head: this value is used only by dequeue, it is either updated
 *   atomically (dequeue_mc) or protected by a mutex (dequeue_sc).
 *
 * - cons_tail: This is used to communicate the latest consumer index between
 *   dequeue and enqueue. Reading a stale value in enqueue can cause an enqueue
 *   to fail erroneously. To avoid a load being re-ordered after a store (and
 *   thus permitting enqueue to store a new value before the old one has been
 *   consumed) it is updated with an atomic_store_rel_32() in deqeueue.
 *
 * - ring[idx] : Updates to this value need to reach memory before the subsequent
 *   update to prod_tail does. Reads need to happen before subsequent updates to
 *   cons_tail.
 *
 * Some implementation notes:
 * - Much like a simpler single-producer single consumer ring buffer,
 *   the producer can not produce faster than the consumer. Hence the
 *   check of 'prod_head' + 1 against 'cons_tail'.
 *
 * - The use of "prod_next = (prod_head + 1) & br->br_prod_mask" to
 *   calculate the next index is slightly cheaper than a modulo but
 *   requires the ring to be power-of-2 sized.
 *
 * - The critical_enter() / critical_exit() are not required for
 *   correctness. They prevent updates from stalling by having a producer be
 *   preempted after updating 'prod_head' but before updating 'prod_tail'.
 *
 * - The "while (br->br_prod_tail != prod_head)"
 *   check assures in order completion (probably not strictly necessary,
 *   but makes it easier to reason about) and allows us to update
 *   'prod_tail' without a cmpxchg / LOCK prefix.
 *
 */
static __inline int
buf_ring_enqueue(struct buf_ring *br, void *buf)
{
	uint32_t prod_head, prod_next, cons_tail;
#ifdef DEBUG_BUFRING
	int i;
	for (i = br->br_cons_head; i != br->br_prod_head;
	     i = ((i + 1) & br->br_cons_mask))
		if(br->br_ring[i].bre_ptr == buf)
			panic("buf=%p already enqueue at %d prod=%d cons=%d",
			    buf, i, br->br_prod_tail, br->br_cons_tail);
#endif	
	critical_enter();
	do {

		prod_head = br->br_prod_head;
		prod_next = (prod_head + 1) & br->br_prod_mask;
		cons_tail = br->br_cons_tail;

		if (prod_next == cons_tail) {
			/* ensure that we only return ENOBUFS
			 * if the latest value matches what we read
			 */
			if (prod_head != atomic_load_acq_32(&br->br_prod_head) ||
			    cons_tail != atomic_load_acq_32(&br->br_cons_tail))
				continue;

			br->br_drops++;
			critical_exit();
			return (ENOBUFS);
		}
	} while (!atomic_cmpset_acq_32(&br->br_prod_head, prod_head, prod_next));
#ifdef DEBUG_BUFRING
	if (br->br_ring[prod_head].bre_ptr != NULL)
		panic("dangling value in enqueue");
#endif	
	br->br_ring[prod_head].bre_ptr = buf;

	/*
	 * If there are other enqueues in progress
	 * that preceded us, we need to wait for them
	 * to complete
	 * re-ordering of reads would not effect correctness
	 */
	while (br->br_prod_tail != prod_head)
		cpu_spinwait();
	/* ensure  that the ring update reaches memory before the new
	 * value of prod_tail
	 */
	atomic_store_rel_32(&br->br_prod_tail, prod_next);
	critical_exit();
	return (0);
}

/*
 * multi-consumer safe dequeue 
 *
 */
static __inline void *
buf_ring_dequeue_mc(struct buf_ring *br)
{
	uint32_t cons_head, cons_next;
	volatile void *buf;

	critical_enter();
	do {
		/*
		 * prod_tail must be read before br_ring[cons_head] is
		 * and the atomic_cmpset_acq_32 on br_cons_head should
		 * enforce that
		 */
		cons_head = br->br_cons_head;
		if (cons_head == br->br_prod_tail) {
			critical_exit();
			return (NULL);
		}
		cons_next = (cons_head + 1) & br->br_cons_mask;
	} while (!atomic_cmpset_acq_32(&br->br_cons_head, cons_head, cons_next));

	/* ensure that the read completes before either of the
	 * subsequent stores
	 */
	buf = br->br_ring[cons_head].bre_ptr;
	/* guarantee that the load completes before we update cons_tail */
	br->br_ring[cons_head].bre_ptr = NULL;

	/*
	 * If there are other dequeues in progress
	 * that preceded us, we need to wait for them
	 * to complete - no memory barrier needed as
	 * re-ordering shouldn't effect correctness or
	 * progress
	 */
	while (br->br_cons_tail != cons_head)
		cpu_spinwait();
	/*
	 * assure that the ring entry is read before
	 * marking the entry as free by updating cons_tail
	 */
	atomic_store_rel_32(&br->br_cons_tail, cons_next);
	critical_exit();

	return ((void *)(uintptr_t)buf);
}

/*
 * single-consumer dequeue 
 * use where dequeue is protected by a lock
 * e.g. a network driver's tx queue lock
 */
static __inline void *
buf_ring_dequeue_sc(struct buf_ring *br)
{
	uint32_t cons_head, cons_next;
#ifdef PREFETCH_DEFINED
	uint32_t cons_next_next;
	uint32_t prod_tail;
#endif
	volatile void *buf;

	/*
	 * prod_tail tells whether or not br_ring[cons_head] is valid
	 * thus we must guarantee that it is read first
	 */
	cons_head = br->br_cons_head;
	if (cons_head == ORDERED_LOAD_32(&br->br_prod_tail))
		return (NULL);

	cons_next = (cons_head + 1) & br->br_cons_mask;
#ifdef PREFETCH_DEFINED
	/*
	 * If prod_tail is stale we will prefetch the wrong value - but this is safe
	 * as cache coherence (should) ensure that the when the value is loaded for
	 * actual use it is fetched from main memory
	 */
	prod_tail = br->br_prod_tail;
	cons_next_next = (cons_head + 2) & br->br_cons_mask;
	if (cons_next != prod_tail) {		
		prefetch(br->br_ring[cons_next].bre_ptr);
		if (cons_next_next != prod_tail) 
			prefetch(br->br_ring[cons_next_next].bre_ptr);
	}
#endif
	br->br_cons_head = cons_next;
	buf = br->br_ring[cons_head].bre_ptr;
	/* guarantee that the load completes before we update cons_tail */
	br->br_ring[cons_head].bre_ptr = NULL;
#ifdef DEBUG_BUFRING
	if (!mtx_owned(br->br_lock))
		panic("lock not held on single consumer dequeue");
	if (br->br_cons_tail != cons_head)
		panic("inconsistent list cons_tail=%d cons_head=%d",
		    br->br_cons_tail, cons_head);
#endif
	atomic_store_rel_32(&br->br_cons_tail, cons_next);

	return ((void *)(uintptr_t)buf);
}

/*
 * single-consumer advance after a peek
 * use where it is protected by a lock
 * e.g. a network driver's tx queue lock
 */
static __inline void
buf_ring_advance_sc(struct buf_ring *br)
{
	uint32_t cons_head, cons_next;
	uint32_t prod_tail;

	cons_head = br->br_cons_head;
	prod_tail = br->br_prod_tail;
	
	cons_next = (cons_head + 1) & br->br_cons_mask;
	if (cons_head == prod_tail) 
		return;
	br->br_cons_head = cons_next;

	/*
	 * Storing NULL here serves two purposes:
	 * 1) it assures that the load of ring[cons_head] has completed
	 *    (only the most perverted architecture or compiler would
	 *    consider re-ordering a = *x; *x = b)
	 * 2) it allows us to enforce global ordering of the cons_tail
	 *    update with an atomic_store_rel_32
	 */
	br->br_ring[cons_head].bre_ptr = NULL;
	atomic_store_rel_32(&br->br_cons_tail, cons_next);
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
 * changed it. As an optimzation we always put it
 * back (since jhb says the store is probably cheaper),
 * if we have to do a multi-queue version we will need
 * the compare and an atomic.
 *
 */
static __inline void
buf_ring_putback_sc(struct buf_ring *br, void *new)
{
	KASSERT(br->br_cons_head != br->br_prod_tail, 
		("Buf-Ring has none in putback")) ;
	br->br_ring[br->br_cons_head].bre_ptr = new;
}

/*
 * return a pointer to the first entry in the ring
 * without modifying it, or NULL if the ring is empty
 * race-prone if not protected by a lock
 */
static __inline void *
buf_ring_peek(struct buf_ring *br)
{
	uint32_t cons_head;
#ifdef DEBUG_BUFRING
	if ((br->br_lock != NULL) && !mtx_owned(br->br_lock))
		panic("lock not held on single consumer dequeue");
#endif	
	cons_head = br->br_cons_head;
	/*
	 * for correctness prod_tail must be read before ring[cons_head]
	 */

	if (cons_head == ORDERED_LOAD_32(&br->br_prod_tail))
		return (NULL);

	/* ensure that the ring load completes before
	 * exposing it to any destructive updates
	 */
	return ((void *)(uintptr_t)br->br_ring[cons_head].bre_ptr);
}

static __inline int
buf_ring_full(struct buf_ring *br)
{
	/* br_cons_tail may be stale but the consumer understands that this is
	* only a point in time snapshot
	*/
	return (((br->br_prod_head + 1) & br->br_prod_mask) == br->br_cons_tail);
}

static __inline int
buf_ring_empty(struct buf_ring *br)
{
	/*  br_prod_tail may be stale but the consumer understands that this is
	*  only a point in time snapshot
	*/

	return (br->br_cons_head == br->br_prod_tail);
}

static __inline int
buf_ring_count(struct buf_ring *br)
{
	/*  br_cons_tail and br_prod_tail may be stale but the consumer
	 * understands that this is only a point in time snapshot
	 */

	return ((br->br_prod_size + br->br_prod_tail - br->br_cons_tail)
	    & br->br_prod_mask);
}

struct buf_ring *buf_ring_alloc(int count, struct malloc_type *type, int flags,
    struct mtx *);
struct buf_ring *buf_ring_aligned_alloc(int count, struct malloc_type *type, int flags,
    struct mtx *);
void buf_ring_free(struct buf_ring *br, struct malloc_type *type);



#endif
