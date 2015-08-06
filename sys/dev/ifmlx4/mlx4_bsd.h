#ifndef _MLX4_BSD_H_
#define  _MLX4_BSD_H_
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>

#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sockio.h>
#include <sys/eventhandler.h>

#include <net/if.h>
#include <net/if_var.h>
#include <net/if_arp.h>
#include <net/bpf.h>
#include <net/ethernet.h>
#include <net/if_dl.h>
#include <net/if_media.h>

#include <net/if_types.h>
#include <net/if_vlan_var.h>
#include <net/iflib.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>


struct mlx4_priv;

struct mlx4_softc {
	struct if_shared_ctx shared;
#define media shared.isc_media
#define hwdev shared.isc_dev
#define hwifp shared.isc_ifp
#define pause_frames shared.isc_pause_frames
#define common_stats shared.isc_common_stats
#define max_frame_size shared.isc_max_frame_size
#define num_queues shared.isc_nqsets

	/*
	 * We keep the linux softc separate to avoid intermingling
	 * of headers
	 */
	struct mlx4_priv *priv;
};


void mlx4_txrx_init(if_shared_ctx_t sctx);

#endif
