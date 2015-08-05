/*-
 * Copyright (c) 2007-2015 Kip Macy <kmacy@freebsd.org>
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

#ifndef	_SYS_BUF_RING_SC_H_
#define	_SYS_BUF_RING_SC_H_

#include <machine/cpu.h>
struct buf_ring_sc;

struct buf_ring_sc_stats_v0 {
	uint64_t brs_enqueues;
	uint64_t brs_drops;
	uint64_t brs_abdications;
	uint64_t brs_stalls;
	uint64_t brs_starts;
	uint64_t brs_restarts;
};

struct buf_ring_sc_consumer {

	/* driver 'drain' function to remove from sw queue and place on hw queue */
	int (*brsc_drain) (struct buf_ring_sc *br, int avail, void *sc);

	/* allow draining to continue in another context */
	void (*brsc_deferred) (struct buf_ring_sc *br, void *sc);

	void *brsc_sc;
	int brsc_domain;
	int brsc_flags;

};

/* cache line align buf ring entries */
#define BR_FLAGS_ALIGNED 0x1
#define BR_FLAGS_NUMA    0x2

struct buf_ring_sc *buf_ring_sc_alloc(int count, struct malloc_type *type, int flags, struct buf_ring_sc_consumer *brsc);
void buf_ring_sc_free(struct buf_ring_sc *br, struct malloc_type *type);
void buf_ring_sc_reset_stats(struct buf_ring_sc *br);
void buf_ring_sc_get_stats_v0(struct buf_ring_sc *br, struct buf_ring_sc_stats_v0 *brss);

/**
 * buf_ring_sc_enqueue - enqueue a buffer to the ring, possibly
 *  acquiring the consumer lock
 * @ents: buffers to enqueue
 * @count: number of buffers
 *
 * return values:
 *  - 0        - success
 *  - ENOBUFS  - failure - could not enqueue all bufs
 */
int buf_ring_sc_enqueue(struct buf_ring_sc *br, void *ents[], int count, int budget);

/**
 * buf_ring_sc_drain - check ring for entries and drain
 * @budget: if non-zero max entries to drain
 * if zero does a blocking acquisition
 *
 */
void buf_ring_sc_drain(struct buf_ring_sc *br, int budget);

/**
 * buf_ring_sc_peek - check ring for entries
 * @ents: array of returned values
 * @count: the size of ents
 *
 * returns: number of entries in ents
 *
 * Populate ents with up to count entries from the ring.
 * returns the number of entries in ents
 * To be used only by the user specified drain function,
 * and only once per-call.
 */
int buf_ring_sc_peek(struct buf_ring_sc *br, void *ents[], uint16_t count);

/**
 * buf_ring_sc_putback - return a buffer to the ring
 * @new: buffer to return
 * @idx: offset from consumer index to return it to
 *
 * Used to return a buffer (most likely already there)
 * to the top of the ring at offset idx from the current
 * consumer index. The caller should *not* have advanced 
 * the index.
 * To be used only by the user specified drain function.
 */
void buf_ring_sc_putback(struct buf_ring_sc *br, void *new, int idx);

/**
 * buf_ring_sc_abdicate - indicate unlock intent
 *
 * Mark the ring as transitioning to unowned.
 * Returns in a critical section. Caller is responsible
 * for calling buf_ring_sc_unlock subsequently with no
 * intervening blocking operations
 * This function is an *optional* optimization to give a
 * wider window for handoff to the next consumer.
  * To be used only by the user specified drain function.
 */
void buf_ring_sc_abdicate(struct buf_ring_sc *br);

int buf_ring_sc_count(struct buf_ring_sc *br);
int buf_ring_sc_empty(struct buf_ring_sc *br);
int buf_ring_sc_full(struct buf_ring_sc *br);
#endif /* BUF_RING_SC_H_ */
