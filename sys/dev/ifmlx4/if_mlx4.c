

#include "ifdi_if.h"
#include "mlx4_pciids.h"
#include "mlx4_bsd.h"


static char mlx4_version[] __devinitdata =
	DRV_NAME ": Mellanox ConnectX core driver v"
	DRV_VERSION " (" DRV_RELDATE ")\n";


/*********************************************************************
 *  Function prototypes
 *********************************************************************/
static int		mlx4_register(device_t);

static int      mlx4_if_attach_pre(if_shared_ctx_t);
static int      mlx4_if_attach_post(if_shared_ctx_t);
static void     mlx4_if_attach_cleanup(if_shared_ctx_t);
static int		mlx4_if_msix_intr_assign(if_shared_ctx_t, int);

static int      mlx4_if_detach(if_shared_ctx_t);

static void		mlx4_if_init(if_shared_ctx_t sctx);
static void		mlx4_if_stop(if_shared_ctx_t sctx);

static void		mlx4_if_intr_enable(if_shared_ctx_t sctx);
static void		mlx4_if_intr_disable(if_shared_ctx_t sctx);
static void		mlx4_if_rx_intr_enable(if_shared_ctx_t sctx, uint16_t rxqid);

static void		mlx4_if_multi_set(if_shared_ctx_t);
static int		mlx4_if_queues_alloc(if_shared_ctx_t, caddr_t *, uint64_t *, int);
static void		mlx4_if_queues_free(if_shared_ctx_t sctx);
static void		mlx4_if_update_admin_status(if_shared_ctx_t);
static int		mlx4_if_mtu_set(if_shared_ctx_t, uint32_t);

static void		mlx4_if_media_status(if_shared_ctx_t, struct ifmediareq *);
static int		mlx4_if_media_change(if_shared_ctx_t);

static void		mlx4_if_timer(if_shared_ctx_t, uint16_t);
static int		mlx4_if_promisc_set(if_shared_ctx_t sctx, int flags);
static void		mlx4_if_vlan_register(if_shared_ctx_t sctx, u16 vtag);
static void		mlx4_if_vlan_unregister(if_shared_ctx_t sctx, u16 vtag);

static device_method_t mlx4_methods[] = {
	/* Device interface */
	DEVMETHOD(device_register, mlx4_register),
	DEVMETHOD(device_probe, iflib_device_probe),
	DEVMETHOD(device_attach, iflib_device_attach),
	DEVMETHOD(device_detach, iflib_device_detach),
	DEVMETHOD(device_shutdown, iflib_device_suspend),
	DEVMETHOD_END
};

static driver_t mlx4_driver = {
	"mlx", mlx4_methods, sizeof(struct mlx4_softc),
};

devclass_t mlx4_devclass;
DRIVER_MODULE(mlx4, pci, mlx4_driver, mlx4_devclass, 0, 0);

MODULE_DEPEND(mlx4, pci, 1, 1, 1);
MODULE_DEPEND(mlx4, ether, 1, 1, 1);
MODULE_DEPEND(mlx4, iflib, 1, 1, 1);


static device_method_t mlx4_if_methods[] = {
	DEVMETHOD(ifdi_attach_pre, mlx4_if_attach_pre),
	DEVMETHOD(ifdi_attach_post, mlx4_if_attach_post),
	DEVMETHOD(ifdi_attach_cleanup, mlx4_if_attach_cleanup),
	DEVMETHOD(ifdi_detach, mlx4_if_detach),
	DEVMETHOD(ifdi_init, mlx4_if_init),
	DEVMETHOD(ifdi_stop, mlx4_if_stop),
	DEVMETHOD(ifdi_msix_intr_assign, mlx4_if_msix_intr_assign),
	DEVMETHOD(ifdi_intr_disable, mlx4_if_intr_disable),
	DEVMETHOD(ifdi_intr_enable, mlx4_if_intr_enable),
	DEVMETHOD(ifdi_rx_intr_enable, mlx4_if_rx_intr_enable),
	DEVMETHOD(ifdi_multi_set, mlx4_if_multi_set),
	DEVMETHOD(ifdi_queues_alloc, mlx4_if_queues_alloc),
	DEVMETHOD(ifdi_update_admin_status, mlx4_if_update_admin_status),
	DEVMETHOD(ifdi_mtu_set, mlx4_if_mtu_set),
	DEVMETHOD(ifdi_media_status, mlx4_if_media_status),
	DEVMETHOD(ifdi_media_change, mlx4_if_media_change),
	DEVMETHOD(ifdi_timer, mlx4_if_timer),
	DEVMETHOD(ifdi_promisc_set, mlx4_if_promisc_set),
	DEVMETHOD(ifdi_vlan_register, mlx4_if_vlan_register),
	DEVMETHOD(ifdi_vlan_unregister, mlx4_if_vlan_unregister),
	DEVMETHOD(ifdi_queues_free, mlx4_if_queues_free),
	DEVMETHOD_END
};

static driver_t mlx4_if_driver = {
	"mlx4", mlx4_if_methods, sizeof(struct if_shared_ctx),
};

static int
mlx4_register(device_t dev)
{
	struct ixl_pf	*pf;
	int             error = 0;
	if_shared_ctx_t sctx;

	/* Allocate, clear, and link in our primary soft structure */
	pf = device_get_softc(dev);
	sctx = UPCAST(pf);
	sctx->isc_dev = dev;

	sctx->isc_q_align = PAGE_SIZE;/* max(DBA_ALIGN, PAGE_SIZE) */

	sctx->isc_tx_maxsize = IXL_TSO_SIZE;
	sctx->isc_tx_nsegments = IXL_MAX_TX_SEGS;
	sctx->isc_tx_maxsegsize = PAGE_SIZE*4;

	sctx->isc_rx_maxsize = PAGE_SIZE*4;
	sctx->isc_rx_nsegments = 1;
	sctx->isc_rx_maxsegsize = PAGE_SIZE*4;
	sctx->isc_ntxd = ixl_ringsz;
	sctx->isc_nrxd = ixl_ringsz;
	sctx->isc_nfl = 1;
	sctx->isc_qsizes = malloc(2*sizeof(uint32_t), M_DEVBUF, M_WAITOK);

	sctx->isc_qsizes[0] = roundup2((ixl_ringsz * sizeof(struct i40e_tx_desc)) +
					 sizeof(u32), DBA_ALIGN);
	sctx->isc_qsizes[1] = roundup2(ixl_ringsz *
					 sizeof(union i40e_rx_desc), DBA_ALIGN);

	sctx->isc_nqs = 2;
	mlx4_txrx_init(sctx);

	sctx->isc_msix_bar = PCIR_BAR(IXL_BAR);;
	sctx->isc_admin_intrcnt = 1;
	sctx->isc_legacy_intr = mlx4_intr;
	sctx->isc_driver = &mlx4_if_driver;

	sctx->isc_vendor_id = PCI_VENDOR_ID_MELLANOX;
	sctx->isc_vendor_info = mlx4_pci_table;
	sctx->isc_driver_version = mlx4_driver_version;
	sctx->isc_vendor_strings = mlx4_strings;
	return (0);
}

static int
mlx4_if_attach_pre(if_shared_ctx_t sctx)
{
	return (EINVAL);
}

static int
mlx4_if_attach_post(if_shared_ctx_t sctx)
{
	return (EINVAL);
}

static void
mlx4_if_attach_cleanup(if_shared_ctx_t sctx)
{

}

static int
mlx4_if_msix_intr_assign(if_shared_ctx_t sctx, int msixcnt)
{
	return (EINVAL);
}

static int
mlx4_if_detach(if_shared_ctx_t sctx)
{
	return (EINVAL);
}

static void
mlx4_if_init(if_shared_ctx_t sctx)
{

}

static void
mlx4_if_stop(if_shared_ctx_t sctx)
{

}

static void
mlx4_if_intr_enable(if_shared_ctx_t sctx)
{

}

static void
mlx4_if_intr_disable(if_shared_ctx_t sctx)
{

}

static void
mlx4_if_rx_intr_enable(if_shared_ctx_t sctx, uint16_t rxqid)
{

}

static void
mlx4_if_multi_set(if_shared_ctx_t sctx)
{

}

static int
mlx4_if_queues_alloc(if_shared_ctx_t, caddr_t *vaddrs, uint64_t *paddrs, int nqs)
{

}

static void
mlx4_if_queues_free(if_shared_ctx_t sctx)
{

}

static void
mlx4_if_update_admin_status(if_shared_ctx_t sctx)
{

}

static int
mlx4_if_mtu_set(if_shared_ctx_t sctx, uint32_t mtu)
{
	return (EINVAL);
}

static void
mlx4_if_media_status(if_shared_ctx_t sctx, struct ifmediareq *ifmr)
{

}

static int
mlx4_if_media_change(if_shared_ctx_t sctx)
{
	return (ENODEV);
}

static void
mlx4_if_timer(if_shared_ctx_t sctx, uint16_t qid)
{

}

static int
mlx4_if_promisc_set(if_shared_ctx_t sctx, int flags)
{
	return (EINVAL);
}

static void
mlx4_if_vlan_register(if_shared_ctx_t sctx, u16 vtag)
{

}

static void
mlx4_if_vlan_unregister(if_shared_ctx_t sctx, u16 vtag)
{

}


