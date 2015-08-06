
#include "mlx4_bsd.h"

static int mlx4_isc_txd_encap(if_shared_ctx_t sctx, if_pkt_info_t pi);
static void mlx4_isc_txd_flush(if_shared_ctx_t sctx, uint16_t txqid, uint32_t pidx);
static int mlx4_isc_txd_credits_update(if_shared_ctx_t sctx, uint16_t qid, uint32_t cidx);
static void mlx4_isc_rxd_refill(if_shared_ctx_t sctx, uint16_t rxqid, uint8_t flid __unused,
				   uint32_t pidx, uint64_t *paddrs, caddr_t *vaddrs __unused, uint16_t count);
static void mlx4_isc_rxd_flush(if_shared_ctx_t sctx, uint16_t rxqid, uint8_t flid __unused, uint32_t pidx);
static int mlx4_isc_rxd_available(if_shared_ctx_t sctx, uint16_t rxqid, uint32_t idx);
static int mlx4_isc_rxd_pkt_get(if_shared_ctx_t sctx, if_rxd_info_t ri);


void
mlx4_txrx_init(if_shared_ctx_t sctx)
{
	sctx->isc_txd_encap = mlx4_isc_txd_encap;
	sctx->isc_txd_flush = mlx4_isc_txd_flush;
	sctx->isc_txd_credits_update = mlx4_isc_txd_credits_update;

	sctx->isc_rxd_available = mlx4_isc_rxd_available;
	sctx->isc_rxd_pkt_get = mlx4_isc_rxd_pkt_get;
	sctx->isc_rxd_refill = mlx4_isc_rxd_refill;
	sctx->isc_rxd_flush = mlx4_isc_rxd_flush;
}

static int
mlx4_isc_txd_encap(if_shared_ctx_t sctx, if_pkt_info_t pi)
{
	panic("UNIMPLEMENTED");
	return (EINVAL);
}

static void
mlx4_isc_txd_flush(if_shared_ctx_t sctx, uint16_t txqid, uint32_t pidx)
{
	panic("UNIMPLEMENTED");
}

static int
mlx4_isc_txd_credits_update(if_shared_ctx_t sctx, uint16_t qid, uint32_t cidx)
{
	panic("UNIMPLEMENTED");
	return (0);
}

static void
mlx4_isc_rxd_refill(if_shared_ctx_t sctx, uint16_t rxqid, uint8_t flid __unused,
					uint32_t pidx, uint64_t *paddrs, caddr_t *vaddrs __unused, uint16_t count)
{
	panic("UNIMPLEMENTED");
}

static void
mlx4_isc_rxd_flush(if_shared_ctx_t sctx, uint16_t rxqid, uint8_t flid __unused, uint32_t pidx)
{
	panic("UNIMPLEMENTED");
}

static int
mlx4_isc_rxd_available(if_shared_ctx_t sctx, uint16_t rxqid, uint32_t idx)
{
	panic("UNIMPLEMENTED");
	return (0);
}

static int
mlx4_isc_rxd_pkt_get(if_shared_ctx_t sctx, if_rxd_info_t ri)
{
	panic("UNIMPLEMENTED");
	return (EINVAL);
}

