/*-
 * Copyright (c) 2014, Matthew Macy (kmacy@freebsd.org)
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
#ifndef __IFLIB_H_
#define __IFLIB_H_

#include <sys/kobj.h>
#include <sys/bus.h>
#include <sys/cpuset.h>
#include <machine/bus.h>
#include <sys/bus_dma.h>
#include <sys/nv.h>

struct iflib_ctx;
typedef struct iflib_ctx *if_ctx_t;
struct if_shared_ctx;
typedef struct if_shared_ctx *if_shared_ctx_t;
struct if_int_delay_info;
typedef struct if_int_delay_info  *if_int_delay_info_t;

/*
 * File organization:
 *  - public structures
 *  - iflib accessors
 *  - iflib utility functions
 *  - iflib core functions
 */

typedef struct if_rxd_info {
	uint16_t iri_qsidx;		/* qset index */
	uint16_t iri_vtag;		/* vlan tag - if flag set */
	uint16_t iri_len;		/* packet length */
	uint16_t iri_next_offset; /* 0 for eop */
	uint32_t iri_cidx;		/* consumer index of cq */
	uint32_t iri_flowid;	/* RSS hash for packet */
	int      iri_flags;		/* mbuf flags for packet */
	uint32_t iri_csum_flags; /* m_pkthdr csum flags */
	uint32_t iri_csum_data;	/* m_pkthdr csum data */
	struct mbuf *iri_m;		/* for driver paths that manage their own rx */
	struct ifnet *iri_ifp;	/* some drivers >1 interface per softc */
	uint8_t	 iri_rsstype; /* RSS hash type */
	uint8_t	 iri_pad;		/* any padding in the received data */
	int8_t	 iri_qidx;		/* == -1 -> completion queue event
							 * >=  0 -> free list id
							 */
} *if_rxd_info_t;

typedef struct if_pkt_info {
	struct mbuf			*ipi_m;		/* tx packet */
	bus_dma_segment_t	*ipi_segs;	/* physical addresses */
	uint16_t			ipi_qsidx;	/* queue set index */
	uint16_t			ipi_nsegs;	/* number of segments */
	uint16_t			ipi_ndescs;	/* number of descriptors used by encap */
	uint32_t			ipi_pidx;	/* start pidx for encap */
	uint32_t			ipi_new_pidx;	/* next available pidx post-encap */
} *if_pkt_info_t;

typedef struct if_irq {
	struct resource  *ii_res;
	int               ii_rid;
	void             *ii_tag;
} *if_irq_t;

struct if_int_delay_info {
	if_ctx_t iidi_ctx;	/* Back-pointer to the iflib ctx (softc) */
	int iidi_offset;			/* Register offset to read/write */
	int iidi_value;			/* Current value in usecs */
	struct sysctl_oid *iidi_oidp;
	struct sysctl_req *iidi_req;
};

typedef enum {
	IFLIB_INTR_LEGACY,
	IFLIB_INTR_MSI,
	IFLIB_INTR_MSIX
} iflib_intr_mode_t;

/*
 * This really belongs in pciio.h or some place more general
 * but this is the only consumer for now.
 */
typedef struct pci_vendor_info {
	uint32_t    pvi_vendor_id;
	uint32_t    pvi_device_id;
	uint32_t    pvi_subvendor_id;
	uint32_t    pvi_subdevice_id;
	uint32_t    pvi_class_mask;
	uint32_t    pvi_index;
} pci_vendor_info_t;


typedef struct if_txrx {
	int (*ift_txd_encap) (void *, if_pkt_info_t);
	void (*ift_txd_flush) (void *, uint16_t, uint32_t);
	int (*ift_txd_credits_update) (void *, uint16_t, uint32_t);

	int (*ift_rxd_available) (void *, uint16_t qsidx, uint32_t pidx);
	int (*ift_rxd_pkt_get) (void *, if_rxd_info_t ri);
	void (*ift_rxd_refill) (void * , uint16_t qsidx, uint8_t flidx, uint32_t pidx,
							uint64_t *paddrs, caddr_t *vaddrs, uint16_t count);
	void (*ift_rxd_flush) (void *, uint16_t qsidx, uint8_t flidx, uint32_t pidx);
	int (*ift_legacy_intr) (void *);
} *if_txrx_t;

typedef struct if_softc_ctx {
	int isc_vectors;
	int isc_nqsets;
	int isc_msix_bar;			/* can be model specific - initialize in attach_pre */
	int isc_tx_nsegments;		/* can be model specific - initialize in attach_pre */
	iflib_intr_mode_t isc_intr;
	uint16_t isc_max_frame_size; /* set at init time by driver */
	pci_vendor_info_t isc_vendor_info;	/* set by iflib prior to attach_pre */
} *if_softc_ctx_t;

/*
 * Initialization values for device
 */
struct if_shared_ctx {
	int isc_magic;
	if_txrx_t isc_txrx;
	driver_t *isc_driver;
	int isc_ntxd;
	int isc_nrxd;
	int isc_nfl;
	int isc_flags;
	bus_size_t isc_q_align;
	bus_size_t isc_tx_maxsize;
	bus_size_t isc_tx_maxsegsize;
	bus_size_t isc_rx_maxsize;
	bus_size_t isc_rx_maxsegsize;
	int isc_rx_nsegments;
	int isc_rx_process_limit;


	uint32_t isc_qsizes[8];
	int isc_nqs;
	int isc_admin_intrcnt;

	int isc_tx_reclaim_thresh;

	/* fields necessary for probe */
	pci_vendor_info_t *isc_vendor_info;
	int isc_vendor_id;
	char **isc_vendor_strings;
	char *isc_driver_version;


};

#define IFLIB_MAGIC 0xCAFEF00D

typedef enum {
	IFLIB_INTR_TX,
	IFLIB_INTR_RX,
	IFLIB_INTR_ADMIN,
	IFLIB_INTR_IOV,
} iflib_intr_type_t;

#ifndef ETH_ADDR_LEN
#define ETH_ADDR_LEN 6
#endif


/*
 * Interface has a separate command queue
 */
#define IFLIB_HAS_CQ		0x1
/*
 * Driver has already allocated vectors
 */
#define IFLIB_SKIP_MSIX		0x2


/*
 * field accessors
 */
void *iflib_get_softc(if_ctx_t ctx);

device_t iflib_get_dev(if_ctx_t ctx);

if_t iflib_get_ifp(if_ctx_t ctx);

struct ifmedia *iflib_get_media(if_ctx_t ctx);

if_softc_ctx_t iflib_get_softc_ctx(if_ctx_t ctx);

void iflib_set_mac(if_ctx_t ctx, uint8_t mac[ETHER_ADDR_LEN]);




/*
 * If the driver can plug cleanly in to newbus use these
 */
int iflib_device_probe(device_t);
int iflib_device_attach(device_t);
int iflib_device_detach(device_t);
int iflib_device_suspend(device_t);
int iflib_device_resume(device_t);
int iflib_device_shutdown(device_t);


int iflib_device_iov_init(device_t, uint16_t, const nvlist_t *);
void iflib_device_iov_uninit(device_t);
int iflib_device_iov_add_vf(device_t, uint16_t, const nvlist_t *);

/*
 * If the driver can't plug cleanly in to newbus
 * use these
 */
int iflib_device_register(device_t dev, void *softc, if_shared_ctx_t sctx, if_ctx_t *ctxp);
int iflib_device_deregister(if_ctx_t);



int iflib_irq_alloc(if_ctx_t, if_irq_t, int, driver_filter_t, void *filter_arg, driver_intr_t, void *arg, char *name);
int iflib_irq_alloc_generic(if_ctx_t ctx, if_irq_t irq, int rid,
							iflib_intr_type_t type, driver_filter_t *filter,
							void *filter_arg, int qid, char *name);
void iflib_softirq_alloc_generic(if_ctx_t ctx, int rid, iflib_intr_type_t type,  void *arg, int qid, char *name);

void iflib_irq_free(if_ctx_t ctx, if_irq_t irq);


void iflib_tx_intr_deferred(if_ctx_t ctx, int txqid);
void iflib_rx_intr_deferred(if_ctx_t ctx, int rxqid);
void iflib_admin_intr_deferred(if_ctx_t ctx);
void iflib_iov_intr_deferred(if_ctx_t ctx);


void iflib_link_state_change(if_ctx_t ctx, int linkstate);





struct mtx *iflib_ctx_lock_get(if_ctx_t);
struct mtx *iflib_qset_lock_get(if_ctx_t, uint16_t);

void iflib_led_create(if_ctx_t ctx);

void iflib_add_int_delay_sysctl(if_ctx_t, const char *, const char *,
								if_int_delay_info_t, int, int);

#ifdef DEV_NETMAP
#include <sys/selinfo.h>
#include <net/netmap.h>
#include <dev/netmap/netmap_kern.h>
int iflib_netmap_register(struct netmap_adapter *na, int onoff);
#endif

#endif /*  __IFLIB_H_ */
