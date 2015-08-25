#ifndef IXL_STANDALONE_BUILD
#include "opt_inet.h"
#include "opt_inet6.h"
#include "opt_rss.h"
#endif

#include "ixl.h"
#include "ixl_pf.h"

#ifdef RSS
#include <net/rss_config.h>
#endif

#include "ifdi_if.h"


/*********************************************************************
 *
 *  Media Ioctl callback
 *
 *  This routine is called when the user changes speed/duplex using
 *  media/mediopt option with ifconfig.
 *
 **********************************************************************/
int
ixl_if_media_change(if_ctx_t ctx)
{
	struct ifmedia *ifm = iflib_get_media(ctx);

	INIT_DEBUGOUT("ixl_media_change: begin");

	if (IFM_TYPE(ifm->ifm_media) != IFM_ETHER)
		return (EINVAL);

	if_printf(iflib_get_ifp(ctx), "Media change is currently not supported.\n");
	return (ENODEV);
}

int
ixl_if_queues_alloc(if_ctx_t ctx, caddr_t *vaddrs, uint64_t *paddrs, int nqs)
{
	struct ixl_vsi *vsi = iflib_get_softc(ctx);
	struct ixl_queue *que;
	struct ixl_tx_buf *bufs;
	if_shared_ctx_t sctx;
	int i;

	MPASS(vsi->num_queues > 0);
	MPASS(nqs == 2);
	/* Allocate queue structure memory */
	sctx = iflib_get_sctx(ctx);
	if (!(vsi->queues =
	    (struct ixl_queue *) malloc(sizeof(struct ixl_queue) *
	    vsi->num_queues, M_IXL, M_NOWAIT | M_ZERO))) {
		device_printf(iflib_get_dev(ctx), "Unable to allocate TX ring memory\n");
		return (ENOMEM);
	}
	if ((bufs = malloc(sizeof(*bufs)*sctx->isc_ntxd*vsi->num_queues, M_IXL, M_WAITOK|M_ZERO)) == NULL) {
		free(vsi->queues, M_IXL);
		device_printf(iflib_get_dev(ctx), "failed to allocate sw bufs\n");
		return (ENOMEM);
	}
	
	for (i = 0, que = vsi->queues; i < vsi->num_queues; i++, que++) {
		struct tx_ring		*txr = &que->txr;
		struct rx_ring 		*rxr = &que->rxr;

		que->me = i;
		que->vsi = vsi;

		/* get the virtual and physical address of the hardware queues */
		txr->tail = I40E_QTX_TAIL(que->me);
		txr->tx_base = (struct i40e_tx_desc *)vaddrs[i*2];
		txr->tx_paddr = paddrs[i*2];
		txr->tx_buffers = bufs + i*sctx->isc_ntxd;
		rxr->tail = I40E_QRX_TAIL(que->me);
		rxr->rx_base = (union i40e_rx_desc *)vaddrs[i*2 + 1];
		rxr->rx_paddr = paddrs[i*2 + 1];
		txr->que = rxr->que = que;
	}

	device_printf(iflib_get_dev(ctx), "allocated for %d queues\n", vsi->num_queues);
	return (0);
}

void
ixl_if_queues_free(if_ctx_t ctx)
{
	struct ixl_vsi *vsi = iflib_get_softc(ctx);
	struct ixl_queue *que;

	if ((que = vsi->queues) == NULL)
		return;
	free(que->txr.tx_buffers, M_IXL);
	free(que, M_IXL);
}
