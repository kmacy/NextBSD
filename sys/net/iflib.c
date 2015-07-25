/*-
 * Copyright (c) 2014-2015, Matthew Macy (kmacy@freebsd.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Neither the name of Matthew Macy nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/bus.h>
#include <sys/buf_ring_sc.h>
#include <sys/eventhandler.h>
#include <sys/sockio.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/module.h>
#include <sys/kobj.h>
#include <sys/rman.h>
#include <sys/smp.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/syslog.h>
#include <sys/taskqueue.h>


#include <net/if.h>
#include <net/if_var.h>
#include <net/if_types.h>
#include <net/if_media.h>
#include <net/bpf.h>
#include <net/ethernet.h>

#include <netinet/in.h>
#include <netinet/tcp_lro.h>

#include <machine/bus.h>

#include <vm/vm.h>
#include <vm/pmap.h>

#include <dev/led/led.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

#include <net/iflib.h>

#include "ifdi_if.h"

#undef DEV_NETMAP

/*
 * File organization:
 *  - private structures
 *  - iflib private utility functions
 *  - ifnet functions
 *  - vlan registry and other exported functions
 *  - iflib public core functions
 *
 *
 * Next steps:
 *  - validate the default tx path
 *  - validate the default rx path
 *
 *  - validate queue initialization paths
 *  - validate queue teardown
 *  - validate that all structure fields are initialized

 *  - add rx_buf recycling
 *  - add SW RSS to demux received data packets to buf_rings for deferred processing
 *    look at handling tx ack processing
 *
 */
MALLOC_DEFINE(M_IFLIB, "iflib", "ifnet library");

struct iflib_txq;
typedef struct iflib_txq *iflib_txq_t;
struct iflib_rxq;
typedef struct iflib_rxq *iflib_rxq_t;
struct iflib_qset;
typedef struct iflib_qset *iflib_qset_t;
struct iflib_fl;
typedef struct iflib_fl *iflib_fl_t;

typedef struct iflib_filter_info {
	driver_filter_t *ifi_filter;
	void *ifi_filter_arg;
	struct grouptask *ifi_task;
} *iflib_filter_info_t;

struct iflib_ctx {

	/*
   * Pointer to hardware driver's softc
   */
	if_shared_ctx_t ifc_sctx;
	struct mtx ifc_mtx;
	char ifc_mtx_name[16];
	iflib_txq_t ifc_txqs;
	iflib_rxq_t ifc_rxqs;
	iflib_qset_t ifc_qsets;
	uint32_t ifc_if_flags;
	uint32_t ifc_flags;
	int			ifc_in_detach;

	int ifc_link_state;
	int ifc_link_irq;
	eventhandler_tag ifc_vlan_attach_event;
	eventhandler_tag ifc_vlan_detach_event;
	struct cdev *ifc_led_dev;
	struct resource *ifc_msix_mem;

	struct if_irq ifc_legacy_irq;
	struct grouptask ifc_admin_task;
	struct iflib_filter_info ifc_filter_info;
};
#define LINK_ACTIVE(ctx) ((ctx)->ifc_link_state == LINK_STATE_UP)

typedef struct iflib_dma_info {
	bus_addr_t			idi_paddr;
	caddr_t				idi_vaddr;
	bus_dma_tag_t		idi_tag;
	bus_dmamap_t		idi_map;
	bus_dma_segment_t	idi_seg;
	int					idi_nseg;
	uint32_t			idi_size;
} *iflib_dma_info_t;

struct iflib_qset {
	iflib_dma_info_t ifq_ifdi;
	uint16_t ifq_nhwqs;
};

#define RX_SW_DESC_MAP_CREATED	(1 << 0)
#define TX_SW_DESC_MAP_CREATED	(1 << 1)
#define RX_SW_DESC_INUSE        (1 << 3)
#define TX_SW_DESC_MAPPED       (1 << 4)

typedef struct iflib_sw_desc {
	bus_dmamap_t    ifsd_map;         /* bus_dma map for packet */
	struct mbuf    *ifsd_m;           /* rx: uninitialized mbuf
									   * tx: pkthdr for the packet
									   */
	caddr_t         ifsd_cl;          /* direct cluster pointer for rx */
	int             ifsd_flags;

	struct mbuf		*ifsd_mh;
	struct mbuf		*ifsd_mt;
} *iflib_sd_t;

/* magic number that should be high enough for any hardware */
#define IFLIB_MAX_TX_SEGS 128
#define IFLIB_RX_COPY_THRESH 128
#define IFLIB_QUEUE_IDLE			0
#define IFLIB_QUEUE_HUNG		1
#define IFLIB_QUEUE_WORKING	2

#define IFLIB_BUDGET 64
#define IFLIB_RESTART_BUDGET 8

#define IFC_LEGACY 0x1
#define IFC_QFLUSH 0x2

struct iflib_txq {
	iflib_ctx_t	ift_ctx;
	uint64_t	ift_flags;
	uint32_t	ift_in_use;
	uint32_t	ift_size;
	uint32_t	ift_processed; /* need to have device tx interrupt update this with credits */
	uint32_t	ift_cleaned;
	uint32_t	ift_stop_thres;
	uint32_t	ift_cidx;
	uint32_t	ift_cidx_processed;
	uint32_t	ift_pidx;
	uint32_t	ift_gen;
	uint32_t	ift_db_pending;
	uint32_t	ift_npending;
	uint32_t	ift_tqid;
	uint64_t	ift_tx_direct_packets;
	uint64_t	ift_tx_direct_bytes;
	uint64_t	ift_no_tx_dma_setup;
	uint64_t	ift_no_desc_avail;
	uint64_t	ift_mbuf_defrag_failed;
	uint64_t	ift_tx_irq;
	bus_dma_tag_t		    ift_desc_tag;
	bus_dma_segment_t	ift_segs[IFLIB_MAX_TX_SEGS];
	struct callout	ift_timer;
	struct callout	ift_db_check;

	struct mtx              ift_mtx;
#define MTX_NAME_LEN 16
	char                    ift_mtx_name[MTX_NAME_LEN];
	int                     ift_id;
	iflib_sd_t              ift_sds;
	int                     ift_nbr;
	struct buf_ring_sc        **ift_br;
	struct grouptask		ift_task;
	int			            ift_qstatus;
	int                     ift_active;
	int                     ift_watchdog_time;
	struct iflib_filter_info ift_filter_info;
	iflib_dma_info_t		ift_ifdi;
};

struct iflib_fl {
	uint32_t	ifl_cidx;
	uint32_t	ifl_pidx;
	uint32_t	ifl_gen;
	uint32_t	ifl_size;
	uint32_t	ifl_credits;
	uint32_t	ifl_buf_size;
	int			ifl_cltype;
	uma_zone_t	ifl_zone;

	iflib_sd_t	ifl_sds;
	iflib_rxq_t	ifl_rxq;
	uint8_t		ifl_id;
	iflib_dma_info_t	ifl_ifdi;
	uint64_t	ifl_phys_addrs[256];
	caddr_t		ifl_vm_addrs[256];
};

static inline int
get_inuse(int size, int cidx, int pidx, int gen)
{
	int used;

	if (pidx > cidx)
		used = pidx - cidx;
	else if (pidx < cidx)
		used = size - cidx + pidx;
	else if (gen == 0 && pidx == cidx)
		used = 0;
	else if (gen == 1 && pidx == cidx)
		used = size;
	else
		panic("bad state");

	return (used);
}

#define TXQ_AVAIL(txq) (txq->ift_size - get_inuse(txq->ift_size, txq->ift_cidx, txq->ift_pidx, txq->ift_gen))

typedef struct iflib_global_context {
	struct taskqgroup	*igc_io_tqg;		/* per-cpu taskqueues for io */
	struct taskqgroup	*igc_config_tqg;	/* taskqueue for config operations */
} iflib_global_context_t;

struct iflib_global_context global_ctx, *gctx;

struct iflib_rxq {
	iflib_ctx_t	ifr_ctx;
	uint32_t	ifr_size;
	uint32_t	ifr_cidx;
	uint32_t	ifr_pidx; /* if there is a separate completion queue -
				     * these are the cq cidx and pidx otherwise
					 * these are unused
					 */
	uint32_t	ifr_gen;
	uint64_t	ifr_rx_irq;
	uint16_t	ifr_id;
	int			ifr_lro_enabled;
	iflib_fl_t	ifr_fl;
	uint8_t		ifr_nfl;
	struct lro_ctrl			ifr_lc;
	struct mtx				ifr_mtx;
	char                    ifr_mtx_name[MTX_NAME_LEN];
	struct grouptask        ifr_task;
	bus_dma_tag_t           ifr_desc_tag;
	iflib_dma_info_t		ifr_ifdi;
	struct iflib_filter_info ifr_filter_info;
};


static int enable_msix = 1;

#define mtx_held(m)	(((m)->mtx_lock & ~MTX_FLAGMASK) != (uintptr_t)0)


#define CTX_ACTIVE(ctx) ((if_getdrvflags((ctx)->ifc_sctx->isc_ifp) & IFF_DRV_RUNNING))

#define CTX_LOCK_INIT(_sc, _name)  mtx_init(&(_sc)->ifc_mtx, _name, "iflib ctx lock", MTX_DEF)

#define CTX_LOCK(ctx) mtx_lock(&(ctx)->ifc_mtx)
#define CTX_UNLOCK(ctx) mtx_unlock(&(ctx)->ifc_mtx)
#define CTX_LOCK_DESTROY(ctx) mtx_destroy(&(ctx)->ifc_mtx)

#define SCTX_LOCK(sctx) CTX_LOCK((sctx)->isc_ctx)
#define SCTX_UNLOCK(sctx) CTX_UNLOCK((sctx)->isc_ctx)


#define TX_LOCK(txq)	mtx_lock(&txq->ift_mtx)
#define TX_TRY_LOCK(txq)	mtx_trylock(&txq->ift_mtx)
#define TX_UNLOCK(txq) 	mtx_unlock(&txq->ift_mtx)


/* Our boot-time initialization hook */
static int	iflib_module_event_handler(module_t, int, void *);

static moduledata_t iflib_moduledata = {
	"iflib",
	iflib_module_event_handler,
	NULL
};

DECLARE_MODULE(iflib, iflib_moduledata, SI_SUB_SMP, SI_ORDER_ANY);
MODULE_VERSION(iflib, 1);

TASKQGROUP_DEFINE(if_io_tqg, mp_ncpus, 1);
TASKQGROUP_DEFINE(if_config_tqg, 1, 1);

#ifndef IFLIB_DEBUG_COUNTERS
#ifdef INVARIANTS
#define IFLIB_DEBUG_COUNTERS 1
#else
#define IFLIB_DEBUG_COUNTERS 0
#endif /* !INVARIANTS */
#endif

static SYSCTL_NODE(_net, OID_AUTO, iflib, CTLFLAG_RD, 0,
                   "iflib driver parameters");

static int iflib_min_tx_latency;

SYSCTL_INT(_net_iflib, OID_AUTO, min_tx_latency, CTLFLAG_RW,
		   &iflib_min_tx_latency, 0, "minimize transmit latency at the possibel expense of throughput");


#if IFLIB_DEBUG_COUNTERS

static int iflib_tx_seen;
static int iflib_rx_allocs;
static int iflib_fl_refills;
static int iflib_fl_refills_large;
static int iflib_tx_frees;

SYSCTL_INT(_net_iflib, OID_AUTO, tx_frees, CTLFLAG_RD,
		   &iflib_tx_frees, 0, "# tx frees");
SYSCTL_INT(_net_iflib, OID_AUTO, tx_seen, CTLFLAG_RD,
		   &iflib_tx_seen, 0, "# tx mbufs seen");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_allocs, CTLFLAG_RD,
		   &iflib_rx_allocs, 0, "# rx allocations");
SYSCTL_INT(_net_iflib, OID_AUTO, fl_refills, CTLFLAG_RD,
		   &iflib_fl_refills, 0, "# refills");
SYSCTL_INT(_net_iflib, OID_AUTO, fl_refills_large, CTLFLAG_RD,
		   &iflib_fl_refills_large, 0, "# large refills");


static int iflib_txq_drain_flushing;
static int iflib_txq_drain_oactive;
static int iflib_txq_drain_notready;
static int iflib_txq_drain_encapfail;

SYSCTL_INT(_net_iflib, OID_AUTO, txq_drain_flushing, CTLFLAG_RD,
		   &iflib_txq_drain_flushing, 0, "# drain flushes");
SYSCTL_INT(_net_iflib, OID_AUTO, txq_drain_oactive, CTLFLAG_RD,
		   &iflib_txq_drain_oactive, 0, "# drain oactives");
SYSCTL_INT(_net_iflib, OID_AUTO, txq_drain_notready, CTLFLAG_RD,
		   &iflib_txq_drain_notready, 0, "# drain notready");
SYSCTL_INT(_net_iflib, OID_AUTO, txq_drain_encapfail, CTLFLAG_RD,
		   &iflib_txq_drain_encapfail, 0, "# drain encap fails");


static int iflib_encap_load_mbuf_fail;
static int iflib_encap_txq_avail_fail;
static int iflib_encap_txd_encap_fail;

SYSCTL_INT(_net_iflib, OID_AUTO, encap_load_mbuf_fail, CTLFLAG_RD,
		   &iflib_encap_load_mbuf_fail, 0, "# busdma load failures");
SYSCTL_INT(_net_iflib, OID_AUTO, encap_txq_avail_fail, CTLFLAG_RD,
		   &iflib_encap_txq_avail_fail, 0, "# txq avail failures");
SYSCTL_INT(_net_iflib, OID_AUTO, encap_txd_encap_fail, CTLFLAG_RD,
		   &iflib_encap_txd_encap_fail, 0, "# driver encap failures");

static int iflib_task_fn_rxs;
static int iflib_rx_intr_enables;
static int iflib_fast_intrs;
static int iflib_rx_unavail;
static int iflib_rx_ctx_inactive;
static int iflib_rx_zero_len;
static int iflib_rx_if_input;
static int iflib_rx_mbuf_null;
static int iflib_rxd_flush;

SYSCTL_INT(_net_iflib, OID_AUTO, task_fn_rx, CTLFLAG_RD,
		   &iflib_task_fn_rxs, 0, "# task_fn_rx calls");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_intr_enables, CTLFLAG_RD,
		   &iflib_rx_intr_enables, 0, "# rx intr enables");
SYSCTL_INT(_net_iflib, OID_AUTO, fast_intrs, CTLFLAG_RD,
		   &iflib_fast_intrs, 0, "# fast_intr calls");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_unavail, CTLFLAG_RD,
		   &iflib_rx_unavail, 0, "# times rxeof called with no available data");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_ctx_inactive, CTLFLAG_RD,
		   &iflib_rx_ctx_inactive, 0, "# times rxeof called with inactive context");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_zero_len, CTLFLAG_RD,
		   &iflib_rx_zero_len, 0, "# times rxeof saw zero len mbuf");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_if_input, CTLFLAG_RD,
		   &iflib_rx_if_input, 0, "# times rxeof called if_input");
SYSCTL_INT(_net_iflib, OID_AUTO, rx_mbuf_null, CTLFLAG_RD,
		   &iflib_rx_mbuf_null, 0, "# times rxeof got null mbuf");
SYSCTL_INT(_net_iflib, OID_AUTO, rxd_flush, CTLFLAG_RD,
		   &iflib_rxd_flush, 0, "# times rxd_flush called");

#define DBG_COUNTER_INC(name) atomic_add_int(&(iflib_ ## name), 1)
#else
#define DBG_COUNTER_INC(name)
#endif

#define IFLIB_DEBUG 0


static void iflib_tx_structures_free(if_shared_ctx_t sctx);
static void iflib_rx_structures_free(if_shared_ctx_t sctx);

#if IFLIB_DEBUG
static void *
if_dbg_malloc(unsigned long size, struct malloc_type *type, int flags)
{
	caddr_t p, ptmp;
	char buf[4] = {0, 0, 0, 0};
	int i;

	ptmp = p = malloc(size, type, flags);

	if ((flags & M_ZERO) == 0)
		return (p);

	for (i = 0; i < size; i += 4, ptmp += 4) {
		if (bcmp(buf, ptmp, 4) != 0)
			panic("received non-zero memory from malloc");
	}
	return (p);
}

#define malloc if_dbg_malloc
#endif

#if defined(__i386__) || defined(__amd64__)
static __inline void
prefetch(void *x)
{
	__asm volatile("prefetcht0 %0" :: "m" (*(unsigned long *)x));
}
#else
#define prefetch(x)
#endif

static void
_iflib_dmamap_cb(void *arg, bus_dma_segment_t *segs, int nseg, int err)
{
	if (err)
		return;
	*(bus_addr_t *) arg = segs[0].ds_addr;
}

static int
iflib_dma_alloc(iflib_ctx_t ctx, bus_size_t size, iflib_dma_info_t dma,
				int mapflags)
{
	int err;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	device_t dev = sctx->isc_dev;

	KASSERT(sctx->isc_q_align != 0, ("alignment value not initialized"));

	err = bus_dma_tag_create(bus_get_dma_tag(dev), /* parent */
				sctx->isc_q_align, 0,	/* alignment, bounds */
				BUS_SPACE_MAXADDR,	/* lowaddr */
				BUS_SPACE_MAXADDR,	/* highaddr */
				NULL, NULL,		/* filter, filterarg */
				size,			/* maxsize */
				1,			/* nsegments */
				size,			/* maxsegsize */
				0,			/* flags */
				NULL,			/* lockfunc */
				NULL,			/* lockarg */
				&dma->idi_tag);
	if (err) {
		device_printf(dev,
		    "%s: bus_dma_tag_create failed: %d\n",
		    __func__, err);
		goto fail_0;
	}

	err = bus_dmamem_alloc(dma->idi_tag, (void**) &dma->idi_vaddr,
	    BUS_DMA_NOWAIT | BUS_DMA_COHERENT, &dma->idi_map);
	if (err) {
		device_printf(dev,
		    "%s: bus_dmamem_alloc(%ju) failed: %d\n",
		    __func__, (uintmax_t)size, err);
		goto fail_2;
	}

	dma->idi_paddr = 0;
	err = bus_dmamap_load(dma->idi_tag, dma->idi_map, dma->idi_vaddr,
	    size, _iflib_dmamap_cb, &dma->idi_paddr, mapflags | BUS_DMA_NOWAIT);
	if (err || dma->idi_paddr == 0) {
		device_printf(dev,
		    "%s: bus_dmamap_load failed: %d\n",
		    __func__, err);
		goto fail_3;
	}

	dma->idi_size = size;
	return (0);

fail_3:
	bus_dmamap_unload(dma->idi_tag, dma->idi_map);
fail_2:
	bus_dmamem_free(dma->idi_tag, dma->idi_vaddr, dma->idi_map);
	bus_dma_tag_destroy(dma->idi_tag);
fail_0:
	dma->idi_tag = NULL;

	return (err);
}

static void
iflib_dma_free(iflib_dma_info_t dma)
{
	if (dma->idi_tag == NULL)
		return;
	if (dma->idi_paddr != 0) {
		bus_dmamap_sync(dma->idi_tag, dma->idi_map,
		    BUS_DMASYNC_POSTREAD | BUS_DMASYNC_POSTWRITE);
		bus_dmamap_unload(dma->idi_tag, dma->idi_map);
		dma->idi_paddr = 0;
	}
	if (dma->idi_vaddr != NULL) {
		bus_dmamem_free(dma->idi_tag, dma->idi_vaddr, dma->idi_map);
		dma->idi_vaddr = NULL;
	}
	bus_dma_tag_destroy(dma->idi_tag);
	dma->idi_tag = NULL;
}

static int
iflib_fast_intr(void *arg)
{
	iflib_filter_info_t info = arg;
	struct grouptask *gtask = info->ifi_task;

	DBG_COUNTER_INC(fast_intrs);
	if (info->ifi_filter != NULL && info->ifi_filter(info->ifi_filter_arg) == FILTER_HANDLED)
		return (FILTER_HANDLED);

	GROUPTASK_ENQUEUE(gtask);
	return (FILTER_HANDLED);
}

static int
_iflib_irq_alloc(iflib_ctx_t ctx, if_irq_t irq, int rid,
	driver_filter_t filter, driver_intr_t handler, void *arg,
				 char *name)
{
	int rc;
	struct resource *res;
	void *tag;
	device_t dev = ctx->ifc_sctx->isc_dev;

	irq->ii_rid = rid;
	res = bus_alloc_resource_any(dev, SYS_RES_IRQ, &irq->ii_rid,
	    RF_SHAREABLE | RF_ACTIVE);
	if (res == NULL) {
		device_printf(dev,
		    "failed to allocate IRQ for rid %d, name %s.\n", rid, name);
		return (ENOMEM);
	}
	irq->ii_res = res;
	KASSERT(filter == NULL || handler == NULL, ("filter and handler can't both be non-NULL"));
	rc = bus_setup_intr(dev, res, INTR_MPSAFE | INTR_TYPE_NET,
						filter, handler, arg, &tag);
	if (rc != 0) {
		device_printf(dev,
		    "failed to setup interrupt for rid %d, name %s: %d\n",
					  rid, name ? name : "unknown", rc);
		return (rc);
	} else if (name)
		bus_describe_intr(dev, res, tag, name);

	irq->ii_tag = tag;
	return (0);
}


/*********************************************************************
 *
 *  Allocate memory for tx_buffer structures. The tx_buffer stores all
 *  the information needed to transmit a packet on the wire. This is
 *  called only once at attach, setup is done every reset.
 *
 **********************************************************************/

static int
iflib_txsd_alloc(iflib_txq_t txq)
{
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	device_t dev = sctx->isc_dev;
	iflib_sd_t txsd;
	int err, i;

	MPASS(sctx->isc_ntxd > 0);
	/*
	 * Setup DMA descriptor areas.
	 */
	if ((err = bus_dma_tag_create(bus_get_dma_tag(dev),
			       1, 0,			/* alignment, bounds */
			       BUS_SPACE_MAXADDR,	/* lowaddr */
			       BUS_SPACE_MAXADDR,	/* highaddr */
			       NULL, NULL,		/* filter, filterarg */
			       sctx->isc_tx_maxsize,		/* maxsize */
			       sctx->isc_tx_nsegments,	/* nsegments */
			       sctx->isc_tx_maxsegsize,	/* maxsegsize */
			       0,			/* flags */
			       NULL,			/* lockfunc */
			       NULL,			/* lockfuncarg */
			       &txq->ift_desc_tag))) {
		device_printf(dev,"Unable to allocate TX DMA tag: %d\n", err);
		device_printf(dev,"maxsize: %ld nsegments: %d maxsegsize: %ld\n",
					  sctx->isc_tx_maxsize, sctx->isc_tx_nsegments, sctx->isc_tx_maxsegsize);
		goto fail;
	}

	if (!(txq->ift_sds =
	    (iflib_sd_t) malloc(sizeof(struct iflib_sw_desc) *
	    sctx->isc_ntxd, M_DEVBUF, M_NOWAIT | M_ZERO))) {
		device_printf(dev, "Unable to allocate tx_buffer memory\n");
		err = ENOMEM;
		goto fail;
	}

        /* Create the descriptor buffer dma maps */
	txsd = txq->ift_sds;
	for (i = 0; i < sctx->isc_ntxd; i++, txsd++) {
		err = bus_dmamap_create(txq->ift_desc_tag, 0, &txsd->ifsd_map);
		if (err != 0) {
			device_printf(dev, "Unable to create TX DMA map\n");
			goto fail;
		}
	}

	return 0;
fail:
	/* We free all, it handles case where we are in the middle */
	iflib_tx_structures_free(sctx);
	return (err);
}

/*
 * XXX Review tx cleaning and buffer mapping
 *
 */

static void
iflib_txsd_destroy(iflib_ctx_t ctx, iflib_txq_t txq, iflib_sd_t txsd)
{
	if (txsd->ifsd_m != NULL) {
		if (txsd->ifsd_map != NULL) {
			bus_dmamap_destroy(txq->ift_desc_tag, txsd->ifsd_map);
			txsd->ifsd_map = NULL;
		}
	} else if (txsd->ifsd_map != NULL) {
		bus_dmamap_unload(txq->ift_desc_tag,
						  txsd->ifsd_map);
		bus_dmamap_destroy(txq->ift_desc_tag,
						   txsd->ifsd_map);
		txsd->ifsd_map = NULL;
	}
}

static void
iflib_txq_destroy(iflib_txq_t txq)
{
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	iflib_sd_t sd = txq->ift_sds;

	for (int i = 0; i < sctx->isc_ntxd; i++, sd++)
		iflib_txsd_destroy(ctx, txq, sd);
	if (txq->ift_sds != NULL) {
		free(txq->ift_sds, M_DEVBUF);
		txq->ift_sds = NULL;
	}
	if (txq->ift_desc_tag != NULL) {
		bus_dma_tag_destroy(txq->ift_desc_tag);
		txq->ift_desc_tag = NULL;
	}
}

static void
iflib_txsd_free(iflib_ctx_t ctx, iflib_txq_t txq, iflib_sd_t txsd)
{
	if (txsd->ifsd_m == NULL)
		return;
	bus_dmamap_sync(txq->ift_desc_tag,
				    txsd->ifsd_map,
				    BUS_DMASYNC_POSTWRITE);
	bus_dmamap_unload(txq->ift_desc_tag,
					  txsd->ifsd_map);
	m_freem(txsd->ifsd_m);
	DBG_COUNTER_INC(tx_frees);
	txsd->ifsd_m = NULL;
}

static int
iflib_txq_setup(iflib_txq_t txq)
{
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	iflib_qset_t qset = &ctx->ifc_qsets[txq->ift_id];
	iflib_sd_t txsd;
	iflib_dma_info_t di;
	int i;
#ifdef DEV_NETMAP
	struct netmap_slot *slot;
	struct netmap_adapter *na = netmap_getna(sctx->isc_ifp);
#endif /* DEV_NETMAP */

#ifdef DEV_NETMAP
	slot = netmap_reset(na, NR_TX, txq->ift_id, 0);
#endif /* DEV_NETMAP */

    /* Set number of descriptors available */
	txq->ift_qstatus = IFLIB_QUEUE_IDLE;

	/* Reset indices */
	txq->ift_cidx_processed = txq->ift_pidx = txq->ift_cidx = txq->ift_npending = 0;
	txq->ift_size = sctx->isc_ntxd;

	/* Free any existing tx buffers. */
	txsd = txq->ift_sds;
	for (int i = 0; i < sctx->isc_ntxd; i++, txsd++) {
		iflib_txsd_free(ctx, txq, txsd);
#ifdef DEV_NETMAP
		if (slot) {
			int si = netmap_idx_n2k(&na->tx_rings[txq->ift_id], i);
			uint64_t paddr;
			void *addr;

			addr = PNMB(na, slot + si, &paddr);
			/*
			 * XXX need netmap down call
			 */
			txq->tx_base[i].buffer_addr = htole64(paddr);
			/* reload the map for netmap mode */
			netmap_load_map(na, txq->ift_desc_tag, txsd->ifsd_map, addr);
		}
#endif /* DEV_NETMAP */

	}
	for (i = 0, di = qset->ifq_ifdi; i < qset->ifq_nhwqs; i++, di++)
		bzero((void *)di->idi_vaddr, di->idi_size);

	IFDI_TXQ_SETUP(sctx, txq->ift_id);
	for (i = 0, di = qset->ifq_ifdi; i < qset->ifq_nhwqs; i++, di++)
		bus_dmamap_sync(di->idi_tag, di->idi_map,
						BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);
	return (0);
}

/*********************************************************************
 *
 *  Allocate memory for rx_buffer structures. Since we use one
 *  rx_buffer per received packet, the maximum number of rx_buffer's
 *  that we'll need is equal to the number of receive descriptors
 *  that we've allocated.
 *
 **********************************************************************/
static int
iflib_rxsd_alloc(iflib_rxq_t rxq)
{
	iflib_ctx_t ctx = rxq->ifr_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	device_t dev = sctx->isc_dev;
	iflib_fl_t fl;
	iflib_sd_t	rxsd;
	int			err;

	MPASS(sctx->isc_nrxd > 0);

	fl = rxq->ifr_fl;
	for (int i = 0; i <  rxq->ifr_nfl; i++, fl++) {
		fl->ifl_sds = malloc(sizeof(struct iflib_sw_desc) *
							 sctx->isc_nrxd, M_DEVBUF, M_WAITOK | M_ZERO);
		if (fl->ifl_sds == NULL) {
			device_printf(dev, "Unable to allocate rx sw desc memory\n");
			return (ENOMEM);
		}
		fl->ifl_size = sctx->isc_nrxd; /* this isn't necessarily the same */
		err = bus_dma_tag_create(bus_get_dma_tag(dev), /* parent */
								 1, 0,			/* alignment, bounds */
								 BUS_SPACE_MAXADDR,	/* lowaddr */
								 BUS_SPACE_MAXADDR,	/* highaddr */
								 NULL, NULL,		/* filter, filterarg */
								 sctx->isc_rx_maxsize,	/* maxsize */
								 sctx->isc_rx_nsegments,	/* nsegments */
								 sctx->isc_rx_maxsegsize,	/* maxsegsize */
								 0,			/* flags */
								 NULL,			/* lockfunc */
								 NULL,			/* lockarg */
								 &rxq->ifr_desc_tag);
		if (err) {
			device_printf(dev, "%s: bus_dma_tag_create failed %d\n",
				__func__, err);
			goto fail;
		}

		rxsd = fl->ifl_sds;
		for (int i = 0; i < sctx->isc_nrxd; i++, rxsd++) {
			err = bus_dmamap_create(rxq->ifr_desc_tag, 0, &rxsd->ifsd_map);
			if (err) {
				device_printf(dev, "%s: bus_dmamap_create failed: %d\n",
					__func__, err);
				goto fail;
			}
		}
	}
	return (0);

fail:
	iflib_rx_structures_free(sctx);
	return (err);
}

/**
 *	rxq_refill - refill an rxq  free-buffer list
 *	@ctx: the iflib context
 *	@rxq: the free-list to refill
 *	@n: the number of new buffers to allocate
 *
 *	(Re)populate an rxq free-buffer list with up to @n new packet buffers.
 *	The caller must assure that @n does not exceed the queue's capacity.
 */
static void
_iflib_fl_refill(iflib_ctx_t ctx, iflib_fl_t fl, int n)
{
	struct mbuf *m;
	int pidx = fl->ifl_pidx;
	iflib_sd_t rxsd = &fl->ifl_sds[pidx];
	caddr_t cl;
	int i = 0;
	uint64_t phys_addr;
	if_shared_ctx_t sctx = ctx->ifc_sctx;

	MPASS(n > 0);
	MPASS(fl->ifl_credits >= 0);
	MPASS(fl->ifl_credits + n <= fl->ifl_size);
#ifdef INVARIANTS
	if (pidx < fl->ifl_cidx)
		MPASS(pidx + n <= fl->ifl_cidx);
	if (pidx == fl->ifl_cidx)
		MPASS(fl->ifl_gen == 0);
	if (pidx > fl->ifl_cidx)
		MPASS(n <= fl->ifl_size - pidx + fl->ifl_cidx);
#endif
	DBG_COUNTER_INC(fl_refills);
	if (n > 8)
		DBG_COUNTER_INC(fl_refills_large);

	while (n--) {
		/*
		 * We allocate an uninitialized mbuf + cluster, mbuf is
		 * initialized after rx.
		 *
		 * If the cluster is still set then we know a minimum sized packet was received
		 */
		if ((cl = rxsd->ifsd_cl) == NULL &&
			(cl = rxsd->ifsd_cl = m_cljget(NULL, M_NOWAIT, fl->ifl_buf_size)) == NULL)
			break;
		if ((m = m_gethdr(M_NOWAIT, MT_NOINIT)) == NULL) {
			break;
		}
		DBG_COUNTER_INC(rx_allocs);
#ifdef notyet
		if ((rxsd->ifsd_flags & RX_SW_DESC_MAP_CREATED) == 0) {
			int err;

			if ((err = bus_dmamap_create(fl->ifl_ifdi->idi_tag, 0, &rxsd->ifsd_map))) {
				log(LOG_WARNING, "bus_dmamap_create failed %d\n", err);
				uma_zfree(fl->ifl_zone, cl);
				n = 0;
				goto done;
			}
			rxsd->ifsd_flags |= RX_SW_DESC_MAP_CREATED;
		}
#endif
#if !defined(__i386__) && !defined(__amd64__)
		{
			struct refill_rxq_cb_arg cb_arg;
			cb_arg.error = 0;
			err = bus_dmamap_load(q->ifr_desc_tag, sd->ifsd_map,
		         cl, q->ifr_buf_size, refill_rxq_cb, &cb_arg, 0);

			if (err != 0 || cb_arg.error) {
				/*
				 * !zone_pack ?
				 */
				if (q->zone == zone_pack)
					uma_zfree(q->ifr_zone, cl);
				m_free(m);
				n = 0;
				goto done;
			}
			phys_addr = cb_arg.seg.ds_addr;
		}
#else
		phys_addr = pmap_kextract((vm_offset_t)cl);
#endif
		rxsd->ifsd_flags |= RX_SW_DESC_INUSE;

		MPASS(rxsd->ifsd_m == NULL);
		rxsd->ifsd_cl = cl;
		rxsd->ifsd_m = m;
		fl->ifl_phys_addrs[i] = phys_addr;
		fl->ifl_vm_addrs[i] = cl;
		rxsd++;
		fl->ifl_credits++;
		i++;
		MPASS(fl->ifl_credits <= fl->ifl_size);
		if (++fl->ifl_pidx == fl->ifl_size) {
			fl->ifl_pidx = 0;
			fl->ifl_gen = 1;
			rxsd = fl->ifl_sds;
		}
		if (n == 0 || i == 256) {
			sctx->isc_rxd_refill(sctx, fl->ifl_rxq->ifr_id, fl->ifl_id, pidx,
								 fl->ifl_phys_addrs, fl->ifl_vm_addrs, i);
			i = 0;
			pidx = fl->ifl_pidx;
		}
	}
#if !defined(__i386__) && !defined(__amd64__)
done:
#endif
	DBG_COUNTER_INC(rxd_flush);
	sctx->isc_rxd_flush(sctx, fl->ifl_rxq->ifr_id, fl->ifl_id, fl->ifl_pidx);
}

static __inline void
__iflib_fl_refill_lt(iflib_ctx_t ctx, iflib_fl_t fl, int max)
{
	/* we avoid allowing pidx to catch up with cidx as it confuses ixl */
	uint32_t reclaimable = fl->ifl_size - fl->ifl_credits - 1;
#ifdef INVARIANTS
	uint32_t delta = fl->ifl_size - get_inuse(fl->ifl_size, fl->ifl_cidx, fl->ifl_pidx, fl->ifl_gen) - 1;

	MPASS(fl->ifl_credits <= fl->ifl_size);
	MPASS(reclaimable == delta);
#endif
	if (reclaimable > 0)
		_iflib_fl_refill(ctx, fl, min(max, reclaimable));
}

static void
iflib_fl_bufs_free(iflib_fl_t fl)
{
	uint32_t cidx = fl->ifl_cidx;

	MPASS(fl->ifl_credits >= 0);
	while (fl->ifl_credits) {
		iflib_sd_t d = &fl->ifl_sds[cidx];

		if (d->ifsd_flags & RX_SW_DESC_INUSE) {
			bus_dmamap_unload(fl->ifl_rxq->ifr_desc_tag, d->ifsd_map);
			bus_dmamap_destroy(fl->ifl_rxq->ifr_desc_tag, d->ifsd_map);
			m_init(d->ifsd_m, zone_mbuf, MLEN,
				   M_NOWAIT, MT_DATA, 0);
			uma_zfree(zone_mbuf, d->ifsd_m);
			uma_zfree(fl->ifl_zone, d->ifsd_cl);
		}
		d->ifsd_cl = NULL;
		d->ifsd_m = NULL;
		if (++cidx == fl->ifl_size)
			cidx = 0;
		fl->ifl_credits--;
	}
}

/*********************************************************************
 *
 *  Initialize a receive ring and its buffers.
 *
 **********************************************************************/
static int
iflib_fl_setup(iflib_fl_t fl)
{
	iflib_rxq_t rxq = fl->ifl_rxq;
	iflib_ctx_t ctx = rxq->ifr_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	int			err = 0;
#ifdef DEV_NETMAP
	struct netmap_slot *slot;
	struct netmap_adapter *na = netmap_getna(sctx->isc_ifp);
#endif

	/* Clear the ring contents */
#ifdef DEV_NETMAP
	slot = netmap_reset(na, NR_RX, rxq->ifr_id, 0);
#endif

	/*
	 * XXX don't set the max_frame_size to larger
	 * than the hardware can handle
	 */
	if (sctx->isc_max_frame_size <= 2048)
		fl->ifl_buf_size = MCLBYTES;
	else if (sctx->isc_max_frame_size <= 4096)
		fl->ifl_buf_size = MJUMPAGESIZE;
	else if (sctx->isc_max_frame_size <= 9216)
		fl->ifl_buf_size = MJUM9BYTES;
	else
		fl->ifl_buf_size = MJUM16BYTES;
	fl->ifl_cltype = m_gettype(fl->ifl_buf_size);
	fl->ifl_zone = m_getzone(fl->ifl_buf_size);

	/*
	** Free current RX buffer structs and their mbufs
	*/
	iflib_fl_bufs_free(fl);

	/* Now replenish the mbufs */
	MPASS(fl->ifl_credits == 0);
	_iflib_fl_refill(ctx, fl, fl->ifl_size);
	MPASS(fl->ifl_pidx == 0);
	MPASS(fl->ifl_size == fl->ifl_credits);
	MPASS(fl->ifl_gen == 1);
	/*
	 * handle failure
	 */
	MPASS(rxq != NULL);
	MPASS(rxq->ifr_ifdi != NULL);
	bus_dmamap_sync(rxq->ifr_ifdi->idi_tag, rxq->ifr_ifdi->idi_map,
	    BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);
	return (err);
}

/*********************************************************************
 *
 *  Free receive ring data structures
 *
 **********************************************************************/
static void
iflib_rx_sds_free(iflib_rxq_t rxq)
{

	if (rxq->ifr_fl != NULL) {
		if (rxq->ifr_fl->ifl_sds != NULL)
			free(rxq->ifr_fl->ifl_sds, M_DEVBUF);

		free(rxq->ifr_fl, M_DEVBUF);
		rxq->ifr_fl = NULL;
		rxq->ifr_gen = rxq->ifr_cidx = rxq->ifr_pidx = 0;
	}

	if (rxq->ifr_desc_tag != NULL) {
		bus_dma_tag_destroy(rxq->ifr_desc_tag);
		rxq->ifr_desc_tag = NULL;
	}
}

/*
 * MI independent logic
 *
 */
static void
iflib_timer(void *arg)
{
	iflib_txq_t txq = arg;
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	if_t ifp = sctx->isc_ifp;

	/*
	** Check on the state of the TX queue(s), this
	** can be done without the lock because its RO
	** and the HUNG state will be static if set.
	*/
	IFDI_TIMER(sctx, txq->ift_id);
	if ((txq->ift_qstatus == IFLIB_QUEUE_HUNG) &&
		(sctx->isc_pause_frames == 0))
		goto hung;

	if (TXQ_AVAIL(txq) <= sctx->isc_tx_nsegments)
		GROUPTASK_ENQUEUE(&txq->ift_task);

	sctx->isc_pause_frames = 0;
	callout_reset_on(&txq->ift_timer, hz/2, iflib_timer, txq, txq->ift_timer.c_cpu);
	return;
hung:
	CTX_LOCK(ctx);
	if_setdrvflagbits(sctx->isc_ifp, 0, IFF_DRV_RUNNING);
	device_printf(sctx->isc_dev,  "TX(%d) desc avail = %d, pidx = %d\n",
				  txq->ift_id, TXQ_AVAIL(txq), txq->ift_pidx);

	IFDI_WATCHDOG_RESET(sctx);
	sctx->isc_watchdog_events++;
	sctx->isc_pause_frames = 0;

	/* Set hardware offload abilities */
	if_clearhwassist(ifp);
	if (if_getcapenable(ifp) & IFCAP_TXCSUM)
		if_sethwassistbits(ifp, CSUM_TCP | CSUM_UDP, 0);
	if (if_getcapenable(ifp) & IFCAP_TSO4)
		if_sethwassistbits(ifp, CSUM_TSO, 0);
	if (if_getcapenable(ifp) & IFCAP_TXCSUM_IPV6)
		if_sethwassistbits(ifp,  (CSUM_TCP_IPV6 | CSUM_UDP_IPV6), 0);

	IFDI_INIT(sctx);
	CTX_UNLOCK(ctx);
}

static void
iflib_init_locked(iflib_ctx_t ctx)
{
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	iflib_txq_t txq = ctx->ifc_txqs;
	int i;

	IFDI_INTR_DISABLE(sctx);
	for (i = 0; i < sctx->isc_nqsets; i++, txq++) {
		TX_LOCK(txq);
		callout_stop(&txq->ift_timer);
		callout_stop(&txq->ift_db_check);
		TX_UNLOCK(txq);
	}
	IFDI_INIT(sctx);
	if_setdrvflagbits(sctx->isc_ifp, IFF_DRV_RUNNING, 0);
	IFDI_INTR_ENABLE(sctx);
	txq = ctx->ifc_txqs;
	for (i = 0; i < sctx->isc_nqsets; i++, txq++)
		callout_reset_on(&txq->ift_timer, hz/2, iflib_timer, txq,
			txq->ift_timer.c_cpu);
}

#define FLOG printf("%s called\n", __FUNCTION__)

static int
iflib_media_change(if_t ifp)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);
	int err;

	CTX_LOCK(ctx);
	if ((err = IFDI_MEDIA_CHANGE(ctx->ifc_sctx)) == 0)
		iflib_init_locked(ctx);
	CTX_UNLOCK(ctx);
	return (err);
}

static void
iflib_media_status(if_t ifp, struct ifmediareq *ifmr)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);

	CTX_LOCK(ctx);
	IFDI_UPDATE_ADMIN_STATUS(ctx->ifc_sctx);
	IFDI_MEDIA_STATUS(ctx->ifc_sctx, ifmr);
	CTX_UNLOCK(ctx);
}

static void
iflib_stop(iflib_ctx_t ctx)
{
	iflib_txq_t txq = ctx->ifc_txqs;
	if_shared_ctx_t sctx = ctx->ifc_sctx;

	IFDI_INTR_DISABLE(sctx);
	/* Tell the stack that the interface is no longer active */
	if_setdrvflagbits(sctx->isc_ifp, IFF_DRV_OACTIVE, IFF_DRV_RUNNING);

	/* Wait for curren tx queue users to exit to disarm watchdog timer. */
	for (int i = 0; i < sctx->isc_nqsets; i++, txq++)
		buf_ring_sc_drain(txq->ift_br[0], 0);
	IFDI_STOP(sctx);
}

/*
 * Internal service routines
 */

#if !defined(__i386__) && !defined(__amd64__)
struct rxq_refill_cb_arg {
	int               error;
	bus_dma_segment_t seg;
	int               nseg;
};

static void
_rxq_refill_cb(void *arg, bus_dma_segment_t *segs, int nseg, int error)
{
	struct rxq_refill_cb_arg *cb_arg = arg;
	
	cb_arg->error = error;
	cb_arg->seg = segs[0];
	cb_arg->nseg = nseg;
}
#endif

/*
 * Process one software descriptor
 */
static struct mbuf *
iflib_rxd_pkt_get(iflib_fl_t fl, if_rxd_info_t ri)
{
	iflib_sd_t sd_next, sd = &fl->ifl_sds[fl->ifl_cidx];
	uint32_t flags = M_EXT;
	caddr_t cl;
	struct mbuf *m;
	int cidx_next, len = ri->iri_len;

	MPASS(sd->ifsd_cl != NULL);
	MPASS(sd->ifsd_m != NULL);

	fl->ifl_credits--;
	/* SYNC ? */

	if (ri->iri_len <= IFLIB_RX_COPY_THRESH) {
		m = sd->ifsd_m;
		sd->ifsd_m = NULL;

		flags = (sd->ifsd_mh == NULL) ? M_PKTHDR : 0;
		m_init(m, fl->ifl_zone, fl->ifl_buf_size, M_NOWAIT, MT_DATA, flags);
		cl = mtod(m, void *);
		memcpy(cl, sd->ifsd_cl, ri->iri_len);
	} else {
		bus_dmamap_unload(fl->ifl_rxq->ifr_desc_tag, sd->ifsd_map);
		cl = sd->ifsd_cl;
		m = sd->ifsd_m;
		sd->ifsd_cl = NULL;
		sd->ifsd_m = NULL;

		if (sd->ifsd_mh == NULL)
			flags |= M_PKTHDR;
		m_init(m, fl->ifl_zone, fl->ifl_buf_size, M_NOWAIT, MT_DATA, flags);
		m_cljset(m, cl, fl->ifl_cltype);
	}

	if (ri->iri_pad) {
		m->m_data += ri->iri_pad;
		len -= ri->iri_pad;
	}
	m->m_len = len;
	if (sd->ifsd_mh == NULL)
		m->m_pkthdr.len = len;
	else
		sd->ifsd_mh->m_pkthdr.len += len;

	if (sd->ifsd_mh != NULL && 	ri->iri_next_offset != 0) {
		/* We're in the middle of a packet and thus
		 * need to pass this packet's data on to the
		 * next descriptor
		 */
		cidx_next = ri->iri_cidx + ri->iri_next_offset;
		if (cidx_next >= fl->ifl_size)
			cidx_next -= fl->ifl_size;
		sd_next = &fl->ifl_sds[cidx_next];
		sd_next->ifsd_mh = sd->ifsd_mh;
		sd_next->ifsd_mt = sd->ifsd_mt;
		sd->ifsd_mh = sd->ifsd_mt = NULL;
		sd_next->ifsd_mt->m_next = m;
		sd_next->ifsd_mt = m;
		m = NULL;
	} else if (sd->ifsd_mh == NULL && ri->iri_next_offset != 0) {
		/*
		 * We're at the start of a multi-fragment packet
		 */
		cidx_next = ri->iri_cidx + ri->iri_next_offset;
		if (cidx_next >= fl->ifl_size)
			cidx_next -= fl->ifl_size;
		sd_next = &fl->ifl_sds[cidx_next];
		sd_next->ifsd_mh = sd_next->ifsd_mt = m;
		m = NULL;
	} else if (sd->ifsd_mh != NULL && ri->iri_next_offset == 0) {
		/*
		 * We're at the end of a multi-fragment packet
		 */
		sd->ifsd_mt->m_next = m;
		sd->ifsd_mt = m;
		m = sd->ifsd_mh;
		sd->ifsd_mh = sd->ifsd_mt = NULL;
	}
	if (m == NULL)
		return (NULL);

	m->m_pkthdr.rcvif = ri->iri_ifp;
	m->m_flags |= ri->iri_flags;

	if (ri->iri_flags & M_VLANTAG)
		if_setvtag(m, ri->iri_vtag);
	m->m_pkthdr.flowid = ri->iri_flowid;
	M_HASHTYPE_SET(m, ri->iri_rsstype);
	m->m_pkthdr.csum_flags = ri->iri_csum_flags;
	m->m_pkthdr.csum_data = ri->iri_csum_data;
	if_inc_counter(ri->iri_ifp, IFCOUNTER_IBYTES, m->m_pkthdr.len);
	if_inc_counter(ri->iri_ifp, IFCOUNTER_IPACKETS, 1);
	return (m);
}

static bool
iflib_rxeof(iflib_rxq_t rxq, int budget)
{
	iflib_ctx_t ctx = rxq->ifr_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	int avail, fl_cidx, cidx, gen, fl_gen;
	int *cidxp, *genp;
	struct if_rxd_info ri;
	iflib_dma_info_t di;
	int err, budget_left;
	iflib_fl_t fl;
	struct ifnet *ifp;
	struct lro_entry *queued;
	int8_t qidx;
	/*
	 * XXX early demux data packets so that if_input processing only handles
	 * acks in interrupt context
	 */
	struct mbuf *m, *mh, *mt;

#ifdef DEV_NETMAP
	if (netmap_rx_irq(ifp, rxq->ifr_id, &processed)) {
		return (FALSE);
	}
#endif /* DEV_NETMAP */

	ri.iri_qsidx = rxq->ifr_id;
	if (sctx->isc_flags & IFLIB_HAS_CQ) {
		cidxp  = &rxq->ifr_cidx;
		genp =  &rxq->ifr_gen;
	} else {
		cidxp = &rxq->ifr_fl[0].ifl_cidx;
		genp = &rxq->ifr_fl[0].ifl_gen;
	}
	cidx = *cidxp;
	gen = *genp;
	mh = mt = NULL;
	MPASS(budget > 0);

	if ((avail = sctx->isc_rxd_available(sctx, rxq->ifr_id, cidx)) == 0) {
		DBG_COUNTER_INC(rx_unavail);
		return (false);
	}
	for (budget_left = budget; (budget_left > 0) && (avail > 0); budget_left--, avail--) {
		if (__predict_false(!CTX_ACTIVE(ctx))) {
			DBG_COUNTER_INC(rx_ctx_inactive);
			break;
		}
		di = rxq->ifr_ifdi;
		bus_dmamap_sync(di->idi_tag, di->idi_map,
		    BUS_DMASYNC_POSTREAD | BUS_DMASYNC_POSTWRITE);

		ri.iri_cidx = cidx;
		/*
		 * Reset client set fields to their default values
		 */
		ri.iri_flags = 0;
		ri.iri_m = NULL;
		ri.iri_next_offset = 0;
		ri.iri_pad = 0;
		ri.iri_qidx = 0;
		ri.iri_ifp = sctx->isc_ifp;
		err = sctx->isc_rxd_pkt_get(sctx, &ri);

		/* in lieu of handling correctly - make sure it isn't being unhandled */
		MPASS(err == 0);

		qidx = ri.iri_qidx;
		if (++cidx == sctx->isc_nrxd) {
			cidx = 0;
			gen = 0;
		}
		if (sctx->isc_flags & IFLIB_HAS_CQ) {
			if (ri.iri_m != NULL) {
				m = ri.iri_m;
				ri.iri_m = NULL;
				goto imm_pkt;
			}
			/* was this only a completion queue message? */
			if (qidx == -1)
				continue;
		}
		fl = &rxq->ifr_fl[qidx];
		fl_cidx = fl->ifl_cidx;
		fl_gen = fl->ifl_gen;
		bus_dmamap_unload(rxq->ifr_desc_tag, fl->ifl_sds[fl_cidx].ifsd_map);

		if (ri.iri_len == 0) {
			DBG_COUNTER_INC(rx_zero_len);
			m_freem(fl->ifl_sds[fl_cidx].ifsd_m);
			fl->ifl_sds[fl_cidx].ifsd_m = NULL;
			/*
			 * XXX Note currently we don't free the initial pieces
			 * of a multi-fragment packet
			 */
			if (++fl_cidx == fl->ifl_size) {
				fl_cidx = 0;
				fl_gen = 0;
			}
			fl->ifl_cidx = fl_cidx;
			fl->ifl_gen = fl_gen;
			continue;
		}
		m = iflib_rxd_pkt_get(fl, &ri);
		if (++fl_cidx == fl->ifl_size) {
			fl_cidx = 0;
			fl_gen = 0;
		}
		fl->ifl_cidx = fl_cidx;
		fl->ifl_gen = fl_gen;

		if (avail == 0 && budget_left)
			avail = sctx->isc_rxd_available(sctx, rxq->ifr_id, cidx);

		if (m == NULL) {
			DBG_COUNTER_INC(rx_mbuf_null);
			continue;
		}
	imm_pkt:
		if (mh == NULL)
			mh = mt = m;
		else {
			mt->m_nextpkt = m;
			mt = m;
		}
	}
	__iflib_fl_refill_lt(ctx, fl, budget);

	ifp = sctx->isc_ifp;
	while (mh != NULL) {
		m = mh;
		mh = mh->m_nextpkt;
		m->m_nextpkt = NULL;
		if (rxq->ifr_lc.lro_cnt != 0 &&
			tcp_lro_rx(&rxq->ifr_lc, m, 0) == 0)
			continue;
		DBG_COUNTER_INC(rx_if_input);
		ifp->if_input(ifp, m);
	}
	/*
	 * Flush any outstanding LRO work
	 */
	while ((queued = SLIST_FIRST(&rxq->ifr_lc.lro_active)) != NULL) {
		SLIST_REMOVE_HEAD(&rxq->ifr_lc.lro_active, next);
		tcp_lro_flush(&rxq->ifr_lc, queued);
	}
#ifdef INVARIANTS
	if ((sctx->isc_flags & IFLIB_HAS_CQ) == 0)
		MPASS(cidx == *cidxp);
#endif
	if (sctx->isc_flags & IFLIB_HAS_CQ)
		*cidxp = cidx;
	return (sctx->isc_rxd_available(sctx, rxq->ifr_id, cidx));
}

#define M_CSUM_FLAGS(m) ((m)->m_pkthdr.csum_flags)
#define M_HAS_VLANTAG(m) (m->m_flags & M_VLANTAG)

static __inline void
iflib_txd_db_check(iflib_ctx_t ctx, iflib_txq_t txq, int ring)
{
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	uint32_t dbval, dbval_prev;

	if (ring || ++txq->ift_db_pending >= 32) {
#ifdef notyet
		iflib_sd_t txsd = &txq->ift_sds[txq->ift_pidx];

		/*
		 * Flush deferred buffers first
		 */
		/* XXX only do this on cards like T3 that can batch packets in a descriptor
		 * and only do this if pidx != cidx
		 */
		if (__predict_false(txsd->ifsd_m != NULL)) {
			struct if_pkt_info pi;

			pi.ipi_m = NULL;
			pi.ipi_qsidx = txq->ift_id;
			pi.ipi_pidx = txq->ift_pidx;
			sctx->isc_txd_encap(sctx, &pi);
			txq->ift_pidx = pi.ipi_new_pidx;
		}
#endif
		dbval_prev = txq->ift_npending ? txq->ift_npending : txq->ift_pidx;
		/* the lock will only ever be contended in the !min_latency case */
		if (TX_TRY_LOCK(txq) == 0)
			return;
		dbval = txq->ift_npending ? txq->ift_npending : txq->ift_pidx;
		if (dbval == dbval_prev) {
			sctx->isc_txd_flush(sctx, txq->ift_id, dbval);
			txq->ift_db_pending = txq->ift_npending = 0;
		}
		TX_UNLOCK(txq);
	}
}

static void
iflib_txd_deferred_db_check(void * arg)
{
	iflib_txq_t txq = arg;
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	uint32_t dbval;

	dbval = txq->ift_npending ? txq->ift_npending : txq->ift_pidx;
	sctx->isc_txd_flush(sctx, txq->ift_id, dbval);
	txq->ift_db_pending = txq->ift_npending = 0;
}

static int
iflib_encap(iflib_txq_t txq, struct mbuf **m_headp)
{
	iflib_ctx_t ctx = txq->ift_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	bus_dma_segment_t	*segs = txq->ift_segs;
	struct mbuf		*m, *m_head = *m_headp;
	int pidx = txq->ift_pidx;
	iflib_sd_t txsd = &txq->ift_sds[pidx];
	bus_dmamap_t		map = txsd->ifsd_map;
	struct if_pkt_info pi;
	bool remap = TRUE;
	int err, nsegs, ndesc;

retry:

	err = bus_dmamap_load_mbuf_sg(txq->ift_desc_tag, map,
	    *m_headp, segs, &nsegs, BUS_DMA_NOWAIT);

	if (__predict_false(err)) {
		switch (err) {
		case EFBIG:
			/* try defrag once */
			if (remap == TRUE) {
				remap = FALSE;
				m = m_defrag(*m_headp, M_NOWAIT);
				if (m == NULL) {
					txq->ift_mbuf_defrag_failed++;
					m_freem(*m_headp);
					DBG_COUNTER_INC(tx_frees);
					*m_headp = NULL;
					err = ENOBUFS;
				} else {
					*m_headp = m;
					goto retry;
				}
			}
			break;
		case ENOMEM:
			txq->ift_no_tx_dma_setup++;
			break;
		default:
			txq->ift_no_tx_dma_setup++;
			m_freem(*m_headp);
			DBG_COUNTER_INC(tx_frees);
			*m_headp = NULL;
			break;
		}
		DBG_COUNTER_INC(encap_load_mbuf_fail);
		return (err);
	}

	/*
	 * XXX assumes a 1 to 1 relationship between segments and
	 *        descriptors - this does not hold true on all drivers, e.g.
	 *        cxgb
	 */
	if (nsegs > TXQ_AVAIL(txq)) {
		txq->ift_no_desc_avail++;
		bus_dmamap_unload(txq->ift_desc_tag, map);
		DBG_COUNTER_INC(encap_txq_avail_fail);
		if (txq->ift_task.gt_task.ta_pending == 0)
			GROUPTASK_ENQUEUE(&txq->ift_task);
		return (ENOBUFS);
	}
	m_head = *m_headp;
	pi.ipi_m = m_head;
	pi.ipi_segs = segs;
	pi.ipi_nsegs = nsegs;
	pi.ipi_pidx = pidx;
	pi.ipi_ndescs = 0;
	pi.ipi_qsidx = txq->ift_id;

	if ((err = sctx->isc_txd_encap(sctx, &pi)) == 0) {
		bus_dmamap_sync(txq->ift_ifdi->idi_tag, txq->ift_ifdi->idi_map,
						BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);

		MPASS(pi.ipi_m != NULL);
		MPASS(txsd->ifsd_m == NULL);
		txsd->ifsd_m = pi.ipi_m;

		if (pi.ipi_new_pidx >= pi.ipi_pidx) {
			ndesc = pi.ipi_new_pidx - pi.ipi_pidx;
		} else {
			ndesc = pi.ipi_new_pidx - pi.ipi_pidx + sctx->isc_ntxd;
			txq->ift_gen = 1;
		}
		txq->ift_in_use += ndesc;
		txq->ift_pidx = pi.ipi_new_pidx;
		txq->ift_npending += pi.ipi_ndescs;
	} else {
		DBG_COUNTER_INC(encap_txd_encap_fail);
	}
	return (err);
}

#define BRBITS 8
#define FIRST_QSET(ctx) 0
#define NQSETS(ctx) ((ctx)->ifc_sctx->isc_nqsets)
#define QIDX(ctx, m) ((((m)->m_pkthdr.flowid >> BRBITS) % NQSETS(ctx)) + FIRST_QSET(ctx))
#define BRIDX(txq, m) ((m)->m_pkthdr.flowid % txq->ift_nbr)
#define DESC_RECLAIMABLE(q) ((int)((q)->ift_processed - (q)->ift_cleaned - (q)->ift_ctx->ifc_sctx->isc_tx_nsegments))
#define RECLAIM_THRESH(ctx) ((ctx)->ifc_sctx->isc_tx_reclaim_thresh)
#define MAX_TX_DESC(ctx) ((ctx)->ifc_sctx->isc_tx_nsegments)



/* if there are more than TXQ_MIN_OCCUPANCY packets pending we consider deferring
 * doorbell writes
 */
#define TXQ_MIN_OCCUPANCY 8

static inline int
iflib_txq_min_occupancy(iflib_txq_t txq)
{

	return (get_inuse(txq->ift_size, txq->ift_cidx, txq->ift_pidx, txq->ift_gen) < TXQ_MIN_OCCUPANCY + MAX_TX_DESC(txq->ift_ctx));
}

static void
iflib_tx_desc_free(iflib_txq_t txq, int n)
{
	iflib_sd_t txsd;
	uint32_t qsize, cidx, mask, gen;
	struct mbuf *m;

	cidx = txq->ift_cidx;
	gen = txq->ift_gen;
	qsize = txq->ift_ctx->ifc_sctx->isc_ntxd;
	mask = qsize-1;
	txsd = &txq->ift_sds[cidx];

	while (n--) {
		prefetch(txq->ift_sds[(cidx + 1) & mask].ifsd_m);
		prefetch(txq->ift_sds[(cidx + 2) & mask].ifsd_m);

		if (txsd->ifsd_m != NULL) {
			if (txsd->ifsd_flags & TX_SW_DESC_MAPPED) {
				bus_dmamap_unload(txq->ift_desc_tag, txsd->ifsd_map);
				txsd->ifsd_flags &= ~TX_SW_DESC_MAPPED;
			}
			while (txsd->ifsd_m) {
				m = txsd->ifsd_m;
				txsd->ifsd_m = m->m_nextpkt;
				m->m_nextpkt = NULL;
				m_freem(m);
				DBG_COUNTER_INC(tx_frees);
			}
		}

		++txsd;
		if (++cidx == qsize) {
			cidx = 0;
			gen = 0;
			txsd = txq->ift_sds;
		}
	}
	txq->ift_cidx = cidx;
	txq->ift_gen = gen;
}

static __inline int
iflib_completed_tx_reclaim(iflib_txq_t txq, int thresh)
{
	int reclaim;
	if_shared_ctx_t sctx = txq->ift_ctx->ifc_sctx;

	KASSERT(thresh >= 0, ("invalid threshold to reclaim"));
	MPASS(thresh + MAX_TX_DESC(txq->ift_ctx) < txq->ift_size);

	reclaim = DESC_RECLAIMABLE(txq);
	/*
	 * Need a rate-limiting check so that this isn't called every time
	 */
	if (sctx->isc_txd_credits_update != NULL && reclaim <= thresh + MAX_TX_DESC(txq->ift_ctx))
		sctx->isc_txd_credits_update(sctx, txq->ift_id, txq->ift_cidx_processed);

	reclaim = DESC_RECLAIMABLE(txq);
	if (reclaim <= thresh + MAX_TX_DESC(txq->ift_ctx))
		return (0);

	iflib_tx_desc_free(txq, reclaim);
	txq->ift_cleaned += reclaim;
	txq->ift_in_use -= reclaim;

	if (txq->ift_active == FALSE)
		txq->ift_active = TRUE;

	return (reclaim);
}

#if 0
static void
iflib_tx_timeout(void *arg)
{

	/* XXX */
}
#endif

static void
iflib_txq_deferred(struct buf_ring_sc *br __unused, void *sc)
{
	iflib_txq_t txq = sc;

	GROUPTASK_ENQUEUE(&txq->ift_task);
}

static int
iflib_txq_drain(struct buf_ring_sc *br, int avail, void *sc)
{
	iflib_txq_t txq = sc;
	iflib_ctx_t ctx = txq->ift_ctx;
	if_t ifp = ctx->ifc_sctx->isc_ifp;
	struct mbuf *mp[16];
	int i, count, pkt_sent, bytes_sent, mcast_sent;

	if (ctx->ifc_flags & IFC_QFLUSH) {
		while ((count = buf_ring_sc_peek(br, (void **)mp, 16)) > 0)
			for (i = 0; i < count; i++)
				m_freem(mp[i]);
		DBG_COUNTER_INC(txq_drain_flushing);
		return (0);
	}
	if (if_getdrvflags(ctx->ifc_sctx->isc_ifp) & IFF_DRV_OACTIVE) {
		txq->ift_qstatus = IFLIB_QUEUE_IDLE;
		TX_LOCK(txq);
		callout_stop(&txq->ift_timer);
		callout_stop(&txq->ift_db_check);
		TX_UNLOCK(txq);
		DBG_COUNTER_INC(txq_drain_oactive);
		return (0);
	}
	mcast_sent = bytes_sent = pkt_sent = 0;
	while (avail) {
		count = buf_ring_sc_peek(br, (void **)mp, MIN(avail, 16));
		KASSERT(count <= MIN(avail, 16), ("invalid count returned"));
		iflib_completed_tx_reclaim(txq, RECLAIM_THRESH(ctx));
		if (!(if_getdrvflags(ifp) & IFF_DRV_RUNNING) ||
			!LINK_ACTIVE(ctx)) {
			DBG_COUNTER_INC(txq_drain_notready);
			goto skip_db;
		}
		for (i = 0; i < count; i++) {
			if(iflib_encap(sc, &mp[i])) {
				buf_ring_sc_putback(br, mp[i], i);
				DBG_COUNTER_INC(txq_drain_encapfail);
				goto done;
			}
			pkt_sent++;
			bytes_sent += mp[i]->m_pkthdr.len;
			if (mp[i]->m_flags & M_MCAST)
				mcast_sent++;
			iflib_txd_db_check(ctx, txq, 0);
			ETHER_BPF_MTAP(ifp, mp[i]);
		}
		avail -= count;
	}
done:

	if ((iflib_min_tx_latency || iflib_txq_min_occupancy(txq)) && txq->ift_db_pending)
		iflib_txd_db_check(ctx, txq, 1);
	else if (txq->ift_db_pending && (callout_pending(&txq->ift_db_check) == 0))
		callout_reset_on(&txq->ift_db_check, 1, iflib_txd_deferred_db_check,
		    txq, txq->ift_db_check.c_cpu);
skip_db:
	if_inc_counter(ifp, IFCOUNTER_OBYTES, bytes_sent);
	if_inc_counter(ifp, IFCOUNTER_OPACKETS, pkt_sent);
	if (mcast_sent)
		if_inc_counter(ifp, IFCOUNTER_OMCASTS, mcast_sent);

	return (pkt_sent);
}

static void
_task_fn_tx(void *context, int pending)
{
	iflib_txq_t txq = context;
	if_shared_ctx_t sctx = txq->ift_ctx->ifc_sctx;

	if (!(if_getdrvflags(sctx->isc_ifp) & IFF_DRV_RUNNING))
		return;

	buf_ring_sc_drain(txq->ift_br[0], IFLIB_BUDGET);
}

static void
_task_fn_rx(void *context, int pending)
{
	iflib_rxq_t rxq = context;
	iflib_ctx_t ctx = rxq->ifr_ctx;
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	bool more;

	DBG_COUNTER_INC(task_fn_rxs);
	if (!(if_getdrvflags(sctx->isc_ifp) & IFF_DRV_RUNNING))
		return;

	if ((more = iflib_rxeof(rxq, 8 /* XXX */)) == false) {
		if (ctx->ifc_flags & IFC_LEGACY)
			IFDI_INTR_ENABLE(sctx);
		else {
			DBG_COUNTER_INC(rx_intr_enables);
			IFDI_RX_INTR_ENABLE(sctx, rxq->ifr_id);
		}
	}
	if (more)
		GROUPTASK_ENQUEUE(&rxq->ifr_task);
}

static void
_task_fn_admin(void *context, int pending)
{
	if_shared_ctx_t sctx = context;
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_txq_t txq;
	int i;

	if (!(if_getdrvflags(sctx->isc_ifp) & IFF_DRV_RUNNING))
		return;

	CTX_LOCK(ctx);
	for (txq = ctx->ifc_txqs, i = 0; i < sctx->isc_nqsets; i++, txq++) {
		TX_LOCK(txq);
		callout_stop(&txq->ift_timer);
		TX_UNLOCK(txq);
	}
	IFDI_UPDATE_ADMIN_STATUS(sctx);
	for (txq = ctx->ifc_txqs, i = 0; i < sctx->isc_nqsets; i++, txq++)
		callout_reset_on(&txq->ift_timer, hz/2, iflib_timer, txq, txq->ift_timer.c_cpu);
	IFDI_LINK_INTR_ENABLE(sctx);
	CTX_UNLOCK(ctx);

	if (LINK_ACTIVE(ctx) == 0)
		return;

	for (txq = ctx->ifc_txqs, i = 0; i < sctx->isc_nqsets; i++, txq++)
		buf_ring_sc_drain(txq->ift_br[0], IFLIB_RESTART_BUDGET);
}

#if 0
void
iflib_intr_rx(void *arg)
{
	iflib_rxq_t rxq = arg;

	++rxq->ifr_rx_irq;
	_task_fn_rx(arg, 0);
}

void
iflib_intr_tx(void *arg)
{
	iflib_txq_t txq= arg;

	++txq->ift_tx_irq;
	_task_fn_tx(arg, 0);
}

void
iflib_intr_link(void *arg)
{
	iflib_ctx_t ctx = arg;

	++ctx->ifc_link_irq;
	_task_fn_link(arg, 0);
}
#endif

static int
iflib_sysctl_int_delay(SYSCTL_HANDLER_ARGS)
{
	int err;
	if_int_delay_info_t info;
	if_shared_ctx_t sctx;

	info = (if_int_delay_info_t)arg1;
	sctx = info->iidi_sctx;
	info->iidi_req = req;
	info->iidi_oidp = oidp;
	SCTX_LOCK(sctx);
	err = IFDI_SYSCTL_INT_DELAY(sctx, info);
	SCTX_UNLOCK(sctx);
	return (err);
}

/*********************************************************************
 *
 *  IFNET FUNCTIONS
 *
 **********************************************************************/

static void
iflib_if_init(void *arg)
{
	iflib_ctx_t ctx = arg;

	iflib_init(ctx->ifc_sctx);
}

static int
iflib_if_transmit(if_t ifp, struct mbuf *m)
{
	iflib_ctx_t	ctx = if_getsoftc(ifp);
	iflib_txq_t txq;
	struct mbuf *marr[16], **mp, *next;
	int err, i, count, qidx;

	DBG_COUNTER_INC(tx_seen);

	if ((ifp->if_drv_flags & IFF_DRV_RUNNING) == 0 || !LINK_ACTIVE(ctx)) {
		DBG_COUNTER_INC(tx_frees);
		m_freem(m);
		return (0);
	}
	qidx = count = 0;
	mp = marr;
	next = m;
	do {
		count++;
		next = next->m_nextpkt;
	} while (next != NULL);

	if (count > 16)
		if ((mp = malloc(count*sizeof(struct mbuf *), M_IFLIB, M_NOWAIT)) == NULL) {
			/* XXX check nextpkt */
			m_freem(m);
			/* XXX simplify for now */
			DBG_COUNTER_INC(tx_frees);
			return (ENOBUFS);
		}
	for (next = m, i = 0; next != NULL; i++) {
		mp[i] = next;
		next = next->m_nextpkt;
		mp[i]->m_nextpkt = NULL;
	}
	if ((NQSETS(ctx) > 1) && M_HASHTYPE_GET(m))
		qidx = QIDX(ctx, m);
	/*
	 * XXX calculate buf_ring based on flowid (divvy up bits?)
	 */
	txq = &ctx->ifc_txqs[qidx];

	err = buf_ring_sc_enqueue(txq->ift_br[0], (void **)mp, count, IFLIB_BUDGET);
	/* drain => err = iflib_txq_transmit(ifp, txq, m); */

	if (count > 16)
		free(mp, M_IFLIB);
	return (err);
}

static void
iflib_if_qflush(if_t ifp)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);
	iflib_txq_t txq = ctx->ifc_txqs;

	ctx->ifc_flags |= IFC_QFLUSH;
	for (int i = 0; i < NQSETS(ctx); i++, txq++)
		buf_ring_sc_drain(txq->ift_br[0], 0);
	if_qflush(ifp);
}

static int
iflib_if_ioctl(if_t ifp, u_long command, caddr_t data)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);
	if_shared_ctx_t sctx = ctx->ifc_sctx;
	struct ifreq	*ifr = (struct ifreq *)data;
#if defined(INET) || defined(INET6)
	struct ifaddr	*ifa = (struct ifaddr *)data;
#endif
	bool		avoid_reset = FALSE;
	int		err = 0;

	switch (command) {
	case SIOCSIFADDR:
#ifdef INET
		if (ifa->ifa_addr->sa_family == AF_INET)
			avoid_reset = TRUE;
#endif
#ifdef INET6
		if (ifa->ifa_addr->sa_family == AF_INET6)
			avoid_reset = TRUE;
#endif
		/*
		** Calling init results in link renegotiation,
		** so we avoid doing it when possible.
		*/
		if (avoid_reset) {
			if_setflagbits(ifp, IFF_UP,0);
			if (!(if_getdrvflags(ifp)& IFF_DRV_RUNNING))
				iflib_init(sctx);
#ifdef INET
			if (!(if_getflags(ifp) & IFF_NOARP))
				arp_ifinit_drv(ifp, ifa);
#endif
		} else
			err = ether_ioctl(ifp, command, data);
		break;
	case SIOCSIFMTU:
		CTX_LOCK(ctx);
		/* detaching ?*/
		if ((err = IFDI_MTU_SET(sctx, ifr->ifr_mtu)) == 0) {
			iflib_init_locked(ctx);
			err = if_setmtu(ifp, ifr->ifr_mtu);
		}
		CTX_UNLOCK(ctx);
		break;
	case SIOCSIFFLAGS:
		CTX_LOCK(ctx);
		if (if_getflags(ifp) & IFF_UP) {
			if (if_getdrvflags(ifp) & IFF_DRV_RUNNING) {
				if ((if_getflags(ifp) ^ ctx->ifc_if_flags) &
				    (IFF_PROMISC | IFF_ALLMULTI)) {
					err = IFDI_PROMISC_SET(sctx, if_getflags(ifp));
				}
			} else
				IFDI_INIT(sctx);
		} else
			if (if_getdrvflags(ifp) & IFF_DRV_RUNNING)
				IFDI_STOP(sctx);
		ctx->ifc_if_flags = if_getflags(ifp);
		CTX_UNLOCK(ctx);
		break;

		break;
	case SIOCADDMULTI:
	case SIOCDELMULTI:
		if (if_getdrvflags(ifp) & IFF_DRV_RUNNING) {
			CTX_LOCK(ctx);
			IFDI_INTR_DISABLE(sctx);
			IFDI_MULTI_SET(sctx);
			IFDI_INTR_ENABLE(sctx);
			CTX_UNLOCK(ctx);
		}
		break;
	case SIOCSIFMEDIA:
		CTX_LOCK(ctx);
		IFDI_MEDIA_SET(sctx);
		CTX_UNLOCK(ctx);
		/* falls thru */
	case SIOCGIFMEDIA:
		err = ifmedia_ioctl(ifp, ifr, &sctx->isc_media, command);
		break;
	case SIOCGI2C:
	{
		struct ifi2creq i2c;

		err = copyin(ifr->ifr_data, &i2c, sizeof(i2c));
		if (err != 0)
			break;
		if (i2c.dev_addr != 0xA0 && i2c.dev_addr != 0xA2) {
			err = EINVAL;
			break;
		}
		if (i2c.len > sizeof(i2c.data)) {
			err = EINVAL;
			break;
		}

		if ((err = IFDI_I2C_REQ(sctx, &i2c)) == 0)
			err = copyout(&i2c, ifr->ifr_data, sizeof(i2c));
		break;
	}
	case SIOCSIFCAP:
	    {
		int mask, reinit;

		reinit = 0;
		mask = ifr->ifr_reqcap ^ if_getcapenable(ifp);

#ifdef TCP_OFFLOAD
		if (mask & IFCAP_TOE4) {
			if_togglecapenable(ifp, IFCAP_TOE4);
			reinit = 1;
		}
#endif
		if (mask & IFCAP_RXCSUM)
			if_togglecapenable(ifp, IFCAP_RXCSUM);
		if (mask & IFCAP_RXCSUM_IPV6)
			if_togglecapenable(ifp, IFCAP_RXCSUM_IPV6);
		if (mask & IFCAP_HWCSUM) {
			if_togglecapenable(ifp, IFCAP_HWCSUM);
			reinit = 1;
		}
		if (mask & IFCAP_LRO)
			if_togglecapenable(ifp, IFCAP_LRO);
		if (mask & IFCAP_TSO4) {
			if_togglecapenable(ifp, IFCAP_TSO4);
			reinit = 1;
		}
		if (mask & IFCAP_TSO6) {
			if_togglecapenable(ifp, IFCAP_TSO6);
			reinit = 1;
		}
		if (mask & IFCAP_VLAN_HWTAGGING) {
			if_togglecapenable(ifp, IFCAP_VLAN_HWTAGGING);
			reinit = 1;
		}
		if (mask & IFCAP_VLAN_MTU) {
			if_togglecapenable(ifp, IFCAP_VLAN_MTU);
			reinit = 1;
		}
		if (mask & IFCAP_VLAN_HWFILTER) {
			if_togglecapenable(ifp, IFCAP_VLAN_HWFILTER);
			reinit = 1;
		}
		if (mask & IFCAP_VLAN_HWTSO) {
			if_togglecapenable(ifp, IFCAP_VLAN_HWTSO);
			reinit = 1;
		}
		if ((mask & IFCAP_WOL) &&
		    (if_getcapabilities(ifp) & IFCAP_WOL) != 0) {
			if (mask & IFCAP_WOL_MCAST)
				if_togglecapenable(ifp, IFCAP_WOL_MCAST);
			if (mask & IFCAP_WOL_MAGIC)
				if_togglecapenable(ifp, IFCAP_WOL_MAGIC);
		}
		if (reinit && (if_getdrvflags(ifp) & IFF_DRV_RUNNING)) {
			iflib_init(sctx);
		}
		if_vlancap(ifp);
		break;
	    }

	default:
		err = ether_ioctl(ifp, command, data);
		break;
	}

	return (err);
}

static uint64_t
iflib_if_get_counter(if_t ifp, ift_counter cnt)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);

	return (IFDI_GET_COUNTER(ctx->ifc_sctx, cnt));
}

uint64_t
iflib_get_counter_default(if_shared_ctx_t sctx, ift_counter cnt)
{
	struct if_common_stats *stats = &sctx->isc_common_stats;

	switch (cnt) {
	case IFCOUNTER_COLLISIONS:
		return (stats->ics_colls);
	case IFCOUNTER_IERRORS:
		return (stats->ics_ierrs);
	case IFCOUNTER_OERRORS:
		return (stats->ics_ierrs);
	default:
		return (if_get_counter_default(sctx->isc_ifp, cnt));
	}
}

/*********************************************************************
 *
 *  OTHER FUNCTIONS EXPORTED TO THE STACK
 *
 **********************************************************************/

static void
iflib_vlan_register(void *arg, if_t ifp, uint16_t vtag)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);

	if ((void *)ctx != arg)
		return;

	if ((vtag == 0) || (vtag > 4095))
		return;

	CTX_LOCK(ctx);
	IFDI_VLAN_REGISTER(ctx->ifc_sctx, vtag);
	/* Re-init to load the changes */
	if (if_getcapenable(ifp) & IFCAP_VLAN_HWFILTER)
		iflib_init_locked(ctx);
	CTX_UNLOCK(ctx);
}

static void
iflib_vlan_unregister(void *arg, if_t ifp, uint16_t vtag)
{
	iflib_ctx_t ctx = if_getsoftc(ifp);

	if ((void *)ctx != arg)
		return;

	if ((vtag == 0) || (vtag > 4095))
		return;

	CTX_LOCK(ctx);
	IFDI_VLAN_UNREGISTER(ctx->ifc_sctx, vtag);
	/* Re-init to load the changes */
	if (if_getcapenable(ifp) & IFCAP_VLAN_HWFILTER)
		iflib_init_locked(ctx);
	CTX_UNLOCK(ctx);
}

static void
iflib_led_func(void *arg, int onoff)
{
	if_shared_ctx_t sctx = arg;

	SCTX_LOCK(sctx);
	IFDI_LED_FUNC(sctx, onoff);
	SCTX_UNLOCK(sctx);
}

/*********************************************************************
 *
 *  BUS FUNCTION DEFINITIONS
 *
 **********************************************************************/

int
iflib_device_detach(device_t dev)
{
	if_shared_ctx_t sctx = device_get_softc(dev);
	iflib_ctx_t ctx = sctx->isc_ctx;
	if_t ifp = sctx->isc_ifp;
	iflib_txq_t txq;
	int i;

	/* Make sure VLANS are not using driver */
	if (if_vlantrunkinuse(ifp)) {
		device_printf(dev,"Vlan in use, detach first\n");
		return (EBUSY);
	}

	CTX_LOCK(ctx);
	ctx->ifc_in_detach = 1;
	iflib_stop(ctx);
	CTX_UNLOCK(ctx);
	CTX_LOCK_DESTROY(ctx);

	/* Unregister VLAN events */
	if (ctx->ifc_vlan_attach_event != NULL)
		EVENTHANDLER_DEREGISTER(vlan_config, ctx->ifc_vlan_attach_event);
	if (ctx->ifc_vlan_detach_event != NULL)
		EVENTHANDLER_DEREGISTER(vlan_unconfig, ctx->ifc_vlan_detach_event);

	ether_ifdetach(sctx->isc_ifp);
	if (ctx->ifc_led_dev != NULL)
		led_destroy(ctx->ifc_led_dev);
	/* XXX drain any dependent tasks */
	IFDI_DETACH(sctx);
	for (txq = ctx->ifc_txqs, i = 0; i < sctx->isc_nqsets; i++, txq++) {
		callout_drain(&txq->ift_timer);
		callout_drain(&txq->ift_db_check);
	}
#ifdef DEV_NETMAP
	netmap_detach(ifp);
#endif /* DEV_NETMAP */

	bus_generic_detach(dev);
	if_free(sctx->isc_ifp);

	iflib_tx_structures_free(sctx);
	iflib_rx_structures_free(sctx);
	return (0);
}

int
iflib_device_suspend(device_t dev)
{
	if_shared_ctx_t sctx = device_get_softc(dev);

	SCTX_LOCK(sctx);
	IFDI_SUSPEND(sctx);
	SCTX_UNLOCK(sctx);

	return bus_generic_suspend(dev);
}

int
iflib_device_resume(device_t dev)
{
	if_shared_ctx_t sctx = device_get_softc(dev);
	iflib_txq_t txq = sctx->isc_ctx->ifc_txqs;

	SCTX_LOCK(sctx);
	IFDI_RESUME(sctx);
	iflib_init_locked(sctx->isc_ctx);
	SCTX_UNLOCK(sctx);
	for (int i = 0; i < sctx->isc_nqsets; i++, txq++)
		buf_ring_sc_drain(txq->ift_br[0], IFLIB_RESTART_BUDGET);

	return (bus_generic_resume(dev));
}

/*********************************************************************
 *
 *  MODULE FUNCTION DEFINITIONS
 *
 **********************************************************************/

/*
 * - Start a fast taskqueue thread for each core
 * - Start a taskqueue for control operations
 */
static int
iflib_module_init(void)
{

	gctx = &global_ctx;
	gctx->igc_io_tqg = qgroup_if_io_tqg;
	gctx->igc_config_tqg = qgroup_if_config_tqg;

	return (0);
}

static int
iflib_module_event_handler(module_t mod, int what, void *arg)
{
	int err;

	switch (what) {
	case MOD_LOAD:
		if ((err = iflib_module_init()) != 0)
			return (err);
		break;
	case MOD_UNLOAD:
		return (EBUSY);
	default:
		return (EOPNOTSUPP);
	}

	return (0);
}

struct buf_ring_sc_consumer brsc = {
	iflib_txq_drain,
	iflib_txq_deferred,
	NULL,
	0
};


/*********************************************************************
 *
 *  PUBLIC FUNCTION DEFINITIONS
 *     ordered as in iflib.h
 *
 **********************************************************************/

void
iflib_hwaddr_set(if_shared_ctx_t sctx, uint8_t addr[ETH_ADDR_LEN])
{
	if_t ifp;

	ifp = sctx->isc_ifp;
	ether_ifattach(ifp, addr);
}


static void
_iflib_assert(if_shared_ctx_t sctx)
{
	MPASS(sctx->isc_tx_maxsize);
	MPASS(sctx->isc_tx_nsegments);
	MPASS(sctx->isc_tx_maxsegsize);

	MPASS(sctx->isc_rx_maxsize);
	MPASS(sctx->isc_rx_nsegments);
	MPASS(sctx->isc_rx_maxsegsize);

	MPASS(sctx->isc_txd_encap);
	MPASS(sctx->isc_txd_flush);
	MPASS(sctx->isc_txd_credits_update);
	MPASS(sctx->isc_rxd_available);
	MPASS(sctx->isc_rxd_pkt_get);
	MPASS(sctx->isc_rxd_refill);
	MPASS(sctx->isc_rxd_flush);
	MPASS(sctx->isc_ntxd > 2*sctx->isc_tx_nsegments);
	MPASS(sctx->isc_nrxd);
}

int
iflib_register(device_t dev, driver_t *driver)
{
	if_shared_ctx_t sctx = device_get_softc(dev);
	iflib_ctx_t ctx;
	if_t ifp;

	_iflib_assert(sctx);
	ctx = malloc(sizeof(struct iflib_ctx), M_DEVBUF, M_ZERO|M_WAITOK);
	if (ctx == NULL)
		return (ENOMEM);
	CTX_LOCK_INIT(ctx, device_get_nameunit(dev));
	sctx->isc_ctx = ctx;
	ctx->ifc_sctx = sctx;

	ifp = sctx->isc_ifp = if_gethandle(IFT_ETHER);
	if (ifp == NULL) {
		device_printf(dev, "can not allocate ifnet structure\n");
		return (ENOMEM);
	}

	/*
	 * Initialize our context's device specific methods
	 */
	kobj_init((kobj_t) sctx, (kobj_class_t) driver);
	kobj_class_compile((kobj_class_t) driver);
	driver->refs++;

	if_initname(ifp, device_get_name(dev), device_get_unit(dev));
	if_setsoftc(ifp, ctx);
	if_setdev(ifp, dev);
	if_setinitfn(ifp, iflib_if_init);
	if_setioctlfn(ifp, iflib_if_ioctl);
	if_settransmitfn(ifp, iflib_if_transmit);
	if_setqflushfn(ifp, iflib_if_qflush);
	if_setgetcounterfn(ifp, iflib_if_get_counter);
	if_setflags(ifp, IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST);

	if_setcapabilities(ifp, 0);
	if_setcapenable(ifp, 0);

	ctx->ifc_vlan_attach_event =
		EVENTHANDLER_REGISTER(vlan_config, iflib_vlan_register, ctx,
							  EVENTHANDLER_PRI_FIRST);
	ctx->ifc_vlan_detach_event =
		EVENTHANDLER_REGISTER(vlan_unconfig, iflib_vlan_unregister, ctx,
							  EVENTHANDLER_PRI_FIRST);

	ifmedia_init(&sctx->isc_media, IFM_IMASK,
					 iflib_media_change, iflib_media_status);

	return (0);
}

int
iflib_queues_alloc(if_shared_ctx_t sctx, uint32_t *qsizes, uint8_t nqs)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	device_t dev = sctx->isc_dev;
	int nqsets = sctx->isc_nqsets;
	iflib_txq_t txq;
	iflib_rxq_t rxq;
	iflib_qset_t qset;
	iflib_fl_t fl = NULL;
	int i, j, err, txconf, rxconf;
	iflib_dma_info_t ifdip;
	int nfree_lists = sctx->isc_nfl ? sctx->isc_nfl : 1;
	struct buf_ring_sc **brscp;
	int nbuf_rings = 1; /* XXX determine dynamically */


	KASSERT(nqs > 0, ("number of queues must be at least 1"));

	if (!(qset =
	    (iflib_qset_t) malloc(sizeof(struct iflib_qset) *
	    nqsets, M_DEVBUF, M_NOWAIT | M_ZERO))) {
		device_printf(dev, "Unable to allocate TX ring memory\n");
		err = ENOMEM;
		goto fail;
	}

/* Allocate the TX ring struct memory */
	if (!(txq =
	    (iflib_txq_t) malloc(sizeof(struct iflib_txq) *
	    nqsets, M_DEVBUF, M_NOWAIT | M_ZERO))) {
		device_printf(dev, "Unable to allocate TX ring memory\n");
		err = ENOMEM;
		goto fail;
	}

	/* Now allocate the RX */
	if (!(rxq =
	    (iflib_rxq_t) malloc(sizeof(struct iflib_rxq) *
	    nqsets, M_DEVBUF, M_NOWAIT | M_ZERO))) {
		device_printf(dev, "Unable to allocate RX ring memory\n");
		err = ENOMEM;
		goto rx_fail;
	}
	if (!(brscp = malloc(sizeof(void *) * nbuf_rings * nqsets, M_DEVBUF, M_NOWAIT | M_ZERO))) {
		device_printf(dev, "Unable to buf_ring_sc * memory\n");
		err = ENOMEM;
		goto rx_fail;
	}

	ctx->ifc_qsets = qset;
	ctx->ifc_txqs = txq;
	ctx->ifc_rxqs = rxq;

	/*
	 * XXX handle allocation failure
	 */
	for (qset = ctx->ifc_qsets, rxconf = txconf = i = 0; i < nqsets;
		 i++, txconf++, rxconf++, qset++, txq++, rxq++) {
		/* Set up some basics */

		if ((ifdip = malloc(sizeof(struct iflib_dma_info) * nqs, M_DEVBUF, M_WAITOK|M_ZERO)) == NULL) {
			device_printf(dev, "failed to allocate iflib_dma_info\n");
			err = ENOMEM;
			goto fail;
		}
		qset->ifq_ifdi = ifdip;
		qset->ifq_nhwqs = nqs;
		for (j = 0; j < nqs; j++, ifdip++) {
			if (iflib_dma_alloc(ctx, qsizes[j], ifdip, BUS_DMA_NOWAIT)) {
				device_printf(dev, "Unable to allocate Descriptor memory\n");
				err = ENOMEM;
				goto err_tx_desc;
			}
			bzero((void *)ifdip->idi_vaddr, qsizes[j]);
		}
		txq->ift_ctx = ctx;
		txq->ift_id = i;
		/* XXX fix this */
		txq->ift_timer.c_cpu = i % mp_ncpus;
		txq->ift_db_check.c_cpu = i % mp_ncpus;
		txq->ift_nbr = nbuf_rings;
		txq->ift_ifdi = &qset->ifq_ifdi[0];

		if (iflib_txsd_alloc(txq)) {
			device_printf(dev,
						  "Critical Failure setting up transmit buffers\n");
			err = ENOMEM;
			goto err_tx_desc;
		}

		/* Initialize the TX lock */
		snprintf(txq->ift_mtx_name, MTX_NAME_LEN, "%s:tx(%d)",
		    device_get_nameunit(dev), txq->ift_id);
		mtx_init(&txq->ift_mtx, txq->ift_mtx_name, NULL, MTX_DEF);
		callout_init_mtx(&txq->ift_timer, &txq->ift_mtx, 0);
		callout_init_mtx(&txq->ift_db_check, &txq->ift_mtx, 0);

		/* Allocate a buf ring */
		brsc.brsc_sc = txq;
		txq->ift_br = brscp + i*nbuf_rings;;
		for (j = 0; j < nbuf_rings; j++)
			if ((txq->ift_br[j] = buf_ring_sc_alloc(4096, M_DEVBUF,
													M_WAITOK, &brsc)) == NULL) {
				device_printf(dev, "Unable to allocate buf_ring\n");
				err = ENOMEM;
				goto fail;
			}
		/*
     * Next the RX queues...
	 */
		rxq->ifr_ctx = ctx;
		rxq->ifr_id = i;
		rxq->ifr_ifdi = &qset->ifq_ifdi[1];
		rxq->ifr_nfl = nfree_lists; 
		if (!(fl =
			  (iflib_fl_t) malloc(sizeof(struct iflib_fl) * nfree_lists, M_DEVBUF, M_NOWAIT | M_ZERO))) {
			device_printf(dev, "Unable to allocate free list memory\n");
			err = ENOMEM;
			goto fail;
		}
		rxq->ifr_fl = fl;
		for (j = 0; j < nfree_lists; j++) {
			rxq->ifr_fl[j].ifl_rxq = rxq;
			rxq->ifr_fl[j].ifl_id = j;
		}
        /* Allocate receive buffers for the ring*/
		if (iflib_rxsd_alloc(rxq)) {
			device_printf(dev,
			    "Critical Failure setting up receive buffers\n");
			err = ENOMEM;
			goto err_rx_desc;
		}

		/* Initialize the RX lock */
		snprintf(rxq->ifr_mtx_name, MTX_NAME_LEN, "%s:rx(%d)",
		    device_get_nameunit(dev), rxq->ifr_id);
		mtx_init(&rxq->ifr_mtx, rxq->ifr_mtx_name, NULL, MTX_DEF);
	}
	if ((err = IFDI_QUEUES_ALLOC(sctx)) != 0) {
		device_printf(sctx->isc_dev, "device queue allocation failed\n");
		iflib_tx_structures_free(sctx);
		return (err);
	}
	return (0);
err_rx_desc:
err_tx_desc:
	if (ctx->ifc_rxqs != NULL)
		free(ctx->ifc_rxqs, M_DEVBUF);
	ctx->ifc_rxqs = NULL;
rx_fail:
	if (ctx->ifc_txqs != NULL)
		free(ctx->ifc_txqs, M_DEVBUF);
	ctx->ifc_txqs = NULL;
fail:
	return (err);
}

static int
iflib_tx_structures_setup(if_shared_ctx_t sctx)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_txq_t txq = ctx->ifc_txqs;
	int i;

	for (i = 0; i < sctx->isc_nqsets; i++, txq++)
		iflib_txq_setup(txq);

	return (0);
}

static void
iflib_tx_structures_free(if_shared_ctx_t sctx)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_txq_t txq = ctx->ifc_txqs;
	iflib_qset_t qset = ctx->ifc_qsets;
	int i, j;

	for (i = 0; i < sctx->isc_nqsets; i++, txq++, qset++) {
		iflib_txq_destroy(txq);
		for (j = 0; j < qset->ifq_nhwqs; j++)
			iflib_dma_free(&qset->ifq_ifdi[j]);
	}
	free(ctx->ifc_txqs, M_DEVBUF);
	free(ctx->ifc_qsets, M_DEVBUF);
	ctx->ifc_txqs = NULL;
	ctx->ifc_qsets = NULL;
	IFDI_QUEUES_FREE(sctx);
}

/*********************************************************************
 *
 *  Initialize all receive rings.
 *
 **********************************************************************/
static int
iflib_rx_structures_setup(if_shared_ctx_t sctx)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_rxq_t rxq = ctx->ifc_rxqs;
	iflib_fl_t fl;
	int i,  q, err;

	for (q = 0; q < sctx->isc_nqsets; q++, rxq++) {
		tcp_lro_free(&rxq->ifr_lc);
		for (i = 0, fl = rxq->ifr_fl; i < rxq->ifr_nfl; i++, fl++)
			if (iflib_fl_setup(fl)) {
				err = ENOBUFS;
				goto fail;
			}
		if (sctx->isc_ifp->if_capenable & IFCAP_LRO) {
			if ((err = tcp_lro_init(&rxq->ifr_lc)) != 0) {
				device_printf(sctx->isc_dev, "LRO Initialization failed!\n");
				goto fail;
			}
			rxq->ifr_lro_enabled = TRUE;
			rxq->ifr_lc.ifp = sctx->isc_ifp;
		}

		IFDI_RXQ_SETUP(sctx, rxq->ifr_id);
	}
	return (0);
fail:
	/*
	 * Free RX software descriptors allocated so far, we will only handle
	 * the rings that completed, the failing case will have
	 * cleaned up for itself. 'q' failed, so its the terminus.
	 */
	rxq = ctx->ifc_rxqs;
	for (i = 0; i < q; ++i, rxq++) {
		iflib_rx_sds_free(rxq);
		rxq->ifr_gen = rxq->ifr_cidx = rxq->ifr_pidx = 0;
	}
	return (err);
}

/*********************************************************************
 *
 *  Free all receive rings.
 *
 **********************************************************************/
static void
iflib_rx_structures_free(if_shared_ctx_t sctx)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_rxq_t rxq = ctx->ifc_rxqs;

	for (int i = 0; i < sctx->isc_nqsets; i++, rxq++) {
		iflib_rx_sds_free(rxq);
	}
}

int
iflib_qset_structures_setup(if_shared_ctx_t sctx)
{
	int err;

	if ((err = iflib_tx_structures_setup(sctx)) != 0)
		return (err);

	if ((err = iflib_rx_structures_setup(sctx)) != 0) {
		device_printf(sctx->isc_dev, "iflib_rx_structures_setup failed: %d\n", err);
		iflib_tx_structures_free(sctx);
		iflib_rx_structures_free(sctx);
	}
	return (err);
}

int
iflib_qset_addr_get(if_shared_ctx_t sctx, int qidx, caddr_t *vaddrs, uint64_t *paddrs, int nqs)
{
	iflib_dma_info_t di = sctx->isc_ctx->ifc_qsets[qidx].ifq_ifdi;
	int i, nhwqs = sctx->isc_ctx->ifc_qsets[qidx].ifq_nhwqs;

	if (nqs != nhwqs)
		return (EINVAL);
	for (i = 0; i < nhwqs; i++, di++) {
		KASSERT(di->idi_vaddr != NULL, ("NULL dma vaddr"));
		KASSERT(di->idi_paddr != 0, ("NULL dma paddr"));
		vaddrs[i] = di->idi_vaddr;
		paddrs[i] = di->idi_paddr;
	}
	return (0);
}

int
iflib_irq_alloc(if_shared_ctx_t sctx, if_irq_t irq, int rid,
				driver_filter_t filter, void *filter_arg, driver_intr_t handler, void *arg, char *name)
{

	return (_iflib_irq_alloc(sctx->isc_ctx, irq, rid, filter, handler, arg, name));
}

int
iflib_irq_alloc_generic(if_shared_ctx_t sctx, if_irq_t irq, int rid,
						iflib_intr_type_t type, driver_filter_t *filter,
						void *filter_arg, int qid, char *name)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	struct grouptask *gtask;
	struct taskqgroup *tqg;
	iflib_filter_info_t info;
	task_fn_t *fn;
	int tqrid;
	void *q;
	int err;
		info = &ctx->ifc_filter_info;

	switch (type) {
	case IFLIB_INTR_TX:
		q = &ctx->ifc_txqs[qid];
		info = &ctx->ifc_txqs[qid].ift_filter_info;
		gtask = &ctx->ifc_txqs[qid].ift_task;
		tqg = gctx->igc_io_tqg;
		tqrid = irq->ii_rid;
		fn = _task_fn_tx;
		break;
	case IFLIB_INTR_RX:
		q = &ctx->ifc_rxqs[qid];
		info = &ctx->ifc_rxqs[qid].ifr_filter_info;
		gtask = &ctx->ifc_rxqs[qid].ifr_task;
		tqg = gctx->igc_io_tqg;
		tqrid = irq->ii_rid;
		fn = _task_fn_rx;
		break;
	case IFLIB_INTR_ADMIN:
		q = sctx;
		info = &ctx->ifc_filter_info;
		gtask = &ctx->ifc_admin_task;
		tqg = gctx->igc_config_tqg;
		tqrid = -1;
		fn = _task_fn_admin;
		break;
	default:
		panic("unknown net intr type");
	}
	GROUPTASK_INIT(gtask, 0, fn, q);

	info->ifi_filter = filter;
	info->ifi_filter_arg = filter_arg;
	info->ifi_task = gtask;

	/* XXX query cpu that rid belongs to */
#ifdef notyet /* how do we iterate - qid!*/
	bus_bind_intr(dev, que->res, cpu_id);
#endif
	err = _iflib_irq_alloc(ctx, irq, rid, iflib_fast_intr, NULL, info,  name);
	if (err != 0)
		return (err);
	taskqgroup_attach(tqg, gtask, q, tqrid, name);
	return (0);
}

void
iflib_softirq_alloc_generic(if_shared_ctx_t sctx, int rid, iflib_intr_type_t type,  void *arg, int qid, char *name)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	struct grouptask *gtask;
	struct taskqgroup *tqg;
	task_fn_t *fn;
	void *q;

	switch (type) {
	case IFLIB_INTR_TX:
		q = &ctx->ifc_txqs[qid];
		gtask = &ctx->ifc_txqs[qid].ift_task;
		tqg = gctx->igc_io_tqg;
		fn = _task_fn_tx;
		break;
	case IFLIB_INTR_RX:
		q = &ctx->ifc_rxqs[qid];
		gtask = &ctx->ifc_rxqs[qid].ifr_task;
		tqg = gctx->igc_io_tqg;
		fn = _task_fn_rx;
		break;
	case IFLIB_INTR_ADMIN:
		q = sctx;
		gtask = &ctx->ifc_admin_task;
		tqg = gctx->igc_config_tqg;
		rid = -1;
		fn = _task_fn_admin;
		break;
	default:
		panic("unknown net intr type");
	}
	GROUPTASK_INIT(gtask, 0, fn, q);
	taskqgroup_attach(tqg, gtask, q, rid, name);
}

void
iflib_irq_free(if_shared_ctx_t sctx, if_irq_t irq)
{
	if (irq->ii_tag)
		bus_teardown_intr(sctx->isc_dev, irq->ii_res, irq->ii_tag);

	if (irq->ii_res)
		bus_release_resource(sctx->isc_dev, SYS_RES_IRQ, irq->ii_rid, irq->ii_res);

	/* taskqgroup_detach() */
}

int
iflib_legacy_setup(if_shared_ctx_t sctx, driver_filter_t filter, void *filterarg, int *rid, char *str)
{
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_txq_t txq = ctx->ifc_txqs;
	iflib_rxq_t rxq = ctx->ifc_rxqs;
	if_irq_t irq = &ctx->ifc_legacy_irq;
	int err;

	ctx->ifc_flags |= IFC_LEGACY;
	/* We allocate a single interrupt resource */
	if ((err = iflib_irq_alloc(sctx, irq, *rid, filter, filterarg, NULL, sctx, str)) != 0)
		return (err);

	/*
	 * Allocate a fast interrupt and the associated
	 * deferred processing contexts.
	 *
	 */
	GROUPTASK_INIT(&txq->ift_task, 0, _task_fn_tx, txq);
	taskqgroup_attach(gctx->igc_io_tqg, &txq->ift_task, txq, irq->ii_rid, "tx");
	GROUPTASK_INIT(&rxq->ifr_task, 0, _task_fn_rx, rxq);
	taskqgroup_attach(gctx->igc_io_tqg, &rxq->ifr_task, rxq, irq->ii_rid, "rx");
	GROUPTASK_INIT(&ctx->ifc_admin_task, 0, _task_fn_admin, sctx);
	taskqgroup_attach(gctx->igc_config_tqg, &ctx->ifc_admin_task, sctx, -1, "admin/link");

	return (0);
}

void
iflib_led_create(if_shared_ctx_t sctx)
{
	iflib_ctx_t ctx = sctx->isc_ctx;

	ctx->ifc_led_dev = led_create(iflib_led_func, sctx,
								  device_get_nameunit(sctx->isc_dev));
}

void
iflib_init(if_shared_ctx_t sctx)
{
	SCTX_LOCK(sctx);
	/* XXX if running stop before init'ing */
	iflib_init_locked(sctx->isc_ctx);
	SCTX_UNLOCK(sctx);
}

void
iflib_tx_intr_deferred(if_shared_ctx_t sctx, int txqid)
{

	GROUPTASK_ENQUEUE(&sctx->isc_ctx->ifc_txqs[txqid].ift_task);
}

void
iflib_rx_intr_deferred(if_shared_ctx_t sctx, int rxqid)
{

	GROUPTASK_ENQUEUE(&sctx->isc_ctx->ifc_rxqs[rxqid].ifr_task);
}

void
iflib_admin_intr_deferred(if_shared_ctx_t sctx)
{

	GROUPTASK_ENQUEUE(&sctx->isc_ctx->ifc_admin_task);
}

void
iflib_link_state_change(if_shared_ctx_t sctx, int link_state)
{
	if_t ifp = sctx->isc_ifp;
	iflib_ctx_t ctx = sctx->isc_ctx;
	iflib_txq_t txq = ctx->ifc_txqs;

#if 0
	if_setbaudrate(ifp, baudrate);
#endif	
	/* If link down, disable watchdog */
	if ((ctx->ifc_link_state == LINK_STATE_UP) && (link_state == LINK_STATE_DOWN)) {
		for (int i = 0; i < sctx->isc_nqsets; i++, txq++)
			txq->ift_qstatus = IFLIB_QUEUE_IDLE;
	}
	ctx->ifc_link_state = link_state;
	if_link_state_change(ifp, link_state);
}

int
iflib_tx_cidx_get(if_shared_ctx_t sctx, int txqid)
{

	return (sctx->isc_ctx->ifc_txqs[txqid].ift_cidx);
}

void
iflib_tx_credits_update(if_shared_ctx_t sctx, int txqid, int credits)
{
	iflib_ctx_t ctx = sctx->isc_ctx;

	ctx->ifc_txqs[txqid].ift_processed += credits;
	ctx->ifc_txqs[txqid].ift_cidx_processed += credits;

	if (ctx->ifc_txqs[txqid].ift_cidx_processed >= ctx->ifc_txqs[txqid].ift_size)
		ctx->ifc_txqs[txqid].ift_cidx_processed -= ctx->ifc_txqs[txqid].ift_size;
}

void
iflib_add_int_delay_sysctl(if_shared_ctx_t sctx, const char *name,
	const char *description, if_int_delay_info_t info,
	int offset, int value)
{
	info->iidi_sctx = sctx;
	info->iidi_offset = offset;
	info->iidi_value = value;
	SYSCTL_ADD_PROC(device_get_sysctl_ctx(sctx->isc_dev),
	    SYSCTL_CHILDREN(device_get_sysctl_tree(sctx->isc_dev)),
	    OID_AUTO, name, CTLTYPE_INT|CTLFLAG_RW,
	    info, 0, iflib_sysctl_int_delay, "I", description);
}

void
iflib_taskqgroup_attach(struct grouptask *gtask, void *uniq, char *name)
{

	taskqgroup_attach(gctx->igc_config_tqg, gtask, uniq, -1, name);
}

struct mtx *
iflib_sctx_lock_get(if_shared_ctx_t sctx)
{

	return (&sctx->isc_ctx->ifc_mtx);
}

int
iflib_msix_init(if_shared_ctx_t sctx, int bar, int admincnt)
{
	device_t dev = sctx->isc_dev;
	int vectors, queues, queuemsgs, msgs;
	iflib_ctx_t ctx = sctx->isc_ctx;
	int err;

	/* Override by tuneable */
	if (enable_msix == 0)
		goto msi;

	/*
	** When used in a virtualized environment 
	** PCI BUSMASTER capability may not be set
	** so explicity set it here and rewrite
	** the ENABLE in the MSIX control register
	** at this point to cause the host to
	** successfully initialize us.
	*/
	{
		uint16_t pci_cmd_word;
		int msix_ctrl, rid;

		rid = 0;
		pci_cmd_word = pci_read_config(dev, PCIR_COMMAND, 2);
		pci_cmd_word |= PCIM_CMD_BUSMASTEREN;
		pci_write_config(dev, PCIR_COMMAND, pci_cmd_word, 2);
		pci_find_cap(dev, PCIY_MSIX, &rid);
		rid += PCIR_MSIX_CTRL;
		msix_ctrl = pci_read_config(dev, rid, 2);
		msix_ctrl |= PCIM_MSIXCTRL_MSIX_ENABLE;
		pci_write_config(dev, rid, msix_ctrl, 2);
	}

	/* First try MSI/X */
	ctx->ifc_msix_mem = bus_alloc_resource_any(dev,
	    SYS_RES_MEMORY, &bar, RF_ACTIVE);
	if (ctx->ifc_msix_mem == NULL) {
		/* May not be enabled */
		device_printf(dev, "Unable to map MSIX table \n");
		goto msi;
	}

	if ((msgs = pci_msix_count(dev)) == 0) { /* system has msix disabled */
		device_printf(dev, "System has MSIX disabled \n");
		bus_release_resource(dev, SYS_RES_MEMORY,
		    bar, ctx->ifc_msix_mem);
		ctx->ifc_msix_mem = NULL;
		goto msi;
	}
#if IFLIB_DEBUG
	/* use only 1 qset in debug mode */
	queuemsgs = min(msgs - admincnt, 1);
#else
	queuemsgs = msgs - admincnt;
#endif

	if (bus_get_cpus(dev, INTR_CPUS, &sctx->isc_cpus) == 0) {
		queues = imin(CPU_COUNT(&sctx->isc_cpus), queuemsgs);
		device_printf(dev, "pxm cpus: %d queue msgs: %d admincnt: %d\n",
					  CPU_COUNT(&sctx->isc_cpus), queuemsgs, admincnt);
#ifdef notyet
		bind_queues = 1;
#endif		
	} else {
		device_printf(dev, "Unable to fetch CPU list\n");
		/* Figure out a reasonable auto config value */
		queues = min(queuemsgs, mp_ncpus);
		device_printf(dev, "using %d queues\n", queues);
	}
#ifdef  RSS
	/* If we're doing RSS, clamp at the number of RSS buckets */
	if (queues > rss_getnumbuckets())
		queues = rss_getnumbuckets();
#endif

	vectors = queues + admincnt;
	if ((err = pci_alloc_msix(dev, &vectors)) == 0) {
		device_printf(dev,
					  "Using MSIX interrupts with %d vectors\n", vectors);
		sctx->isc_vectors = vectors;
		sctx->isc_nqsets = queues;
		sctx->isc_intr = IFLIB_INTR_MSIX;
		return (vectors);
	} else {
		device_printf(dev, "failed to allocate %d msix vectors, err: %d - using MSI\n", vectors, err);
	}
msi:
	vectors = pci_msi_count(dev);
	sctx->isc_nqsets = 1;
	sctx->isc_vectors = vectors;
	if (vectors == 1 && pci_alloc_msi(dev, &vectors) == 0) {
		device_printf(dev,"Using an MSI interrupt\n");
		sctx->isc_intr = IFLIB_INTR_MSI;
	} else {
		device_printf(dev,"Using a Legacy interrupt\n");
		sctx->isc_intr = IFLIB_INTR_LEGACY;
	}
	return (vectors);
}
