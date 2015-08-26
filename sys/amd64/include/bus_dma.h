/*-
 * Copyright (c) 2005 Scott Long
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
 */

#ifndef _AMD64_BUS_DMA_H_
#define _AMD64_BUS_DMA_H_

#include <sys/param.h>
#include <sys/bus_dma.h>
#include <x86/include/busdma_impl.h>

struct bus_dma_tag {
	struct bus_dma_tag_common common;
	int			map_count;
	int			bounce_flags;
	bus_dma_segment_t	*segments;
	struct bounce_zone	*bounce_zone;
};

static __inline bus_dma_segment_t *
_bus_dmamap_complete(bus_dma_tag_t dmat, bus_dmamap_t map,
    bus_dma_segment_t *segs, int nsegs, int error)
{
	struct bus_dma_tag_common *tc;

	tc = (struct bus_dma_tag_common *)dmat;
	if (tc->impl->map_complete != NULL)
		segs = tc->impl->map_complete(dmat, map, segs, nsegs, error);
	else if (__predict_false(segs == NULL))
		segs = dmat->segments;
	return (segs);
}

#endif /* _AMD64_BUS_DMA_H_ */
