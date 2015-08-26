#include "ixgbe.h"

static int ixgbe_isc_txd_encap(void *arg, if_pkt_info_t pi);
static void ixgbe_isc_txd_flush(void *arg, uint16_t txqid, uint32_t pidx);
static int ixgbe_isc_txd_credits_update(void *arg, uint16_t txqid, uint32_t cidx);

static void ixgbe_isc_rxd_refill(void *arg, uint16_t rxqid, uint8_t flid __unused,
				   uint32_t pidx, uint64_t *paddrs, caddr_t *vaddrs __unused, uint16_t count);
static void ixgbe_isc_rxd_flush(void *arg, uint16_t rxqid, uint8_t flid __unused, uint32_t pidx);
static int ixgbe_isc_rxd_available(void *arg, uint16_t rxqid, uint32_t idx);
static int ixgbe_isc_rxd_pkt_get(void *arg, if_rxd_info_t ri);

extern int ixgbe_intr(void *arg);

struct if_txrx ixgbe_txrx  = {
	ixgbe_isc_txd_encap,
	ixgbe_isc_txd_flush,
	ixgbe_isc_txd_credits_update,
	ixgbe_isc_rxd_available,
	ixgbe_isc_rxd_pkt_get,
	ixgbe_isc_rxd_refill,
	ixgbe_isc_rxd_flush,
	ixgbe_intr
};

extern if_shared_ctx_t ixgbe_sctx;

/*********************************************************************
 *
 *  Advanced Context Descriptor setup for VLAN, CSUM or TSO
 *
 **********************************************************************/
static int
ixgbe_tx_ctx_setup(struct tx_ring *txr, struct mbuf *mp,
		   u32 *cmd_type_len, u32 *olinfo_status, int pidx, int *offload)
{
	struct adapter *adapter = txr->adapter;
	struct ixgbe_adv_tx_context_desc *TXD;
	struct ether_vlan_header *eh;
	struct ip *ip;
	struct ip6_hdr *ip6;
	u32 vlan_macip_lens = 0, type_tucmd_mlhl = 0;
	int	ehdrlen, ip_hlen = 0;
	u16	etype;
	u8	ipproto = 0;
	u16	vtag = 0;

	*offload = TRUE;
	/* First check if TSO is to be used */
	if (mp->m_pkthdr.csum_flags & CSUM_TSO)
		return (ixgbe_tso_setup(txr, mp, cmd_type_len, olinfo_status));

	if ((mp->m_pkthdr.csum_flags & CSUM_OFFLOAD) == 0)
		*offload = FALSE;

	/* Indicate the whole packet as payload when not doing TSO */
       	*olinfo_status |= mp->m_pkthdr.len << IXGBE_ADVTXD_PAYLEN_SHIFT;

	/* Now ready a context descriptor */
	TXD = (struct ixgbe_adv_tx_context_desc *) &txr->tx_base[ctxd];

	/*
	** In advanced descriptors the vlan tag must 
	** be placed into the context descriptor. Hence
	** we need to make one even if not doing offloads.
	*/
	if (mp->m_flags & M_VLANTAG) {
		vtag = htole16(mp->m_pkthdr.ether_vtag);
		vlan_macip_lens |= (vtag << IXGBE_ADVTXD_VLAN_SHIFT);
	} else if (!IXGBE_IS_X550VF(adapter) && (*offload == FALSE))
		return (0);

	/*
	 * Determine where frame payload starts.
	 * Jump over vlan headers if already present,
	 * helpful for QinQ too.
	 */
	eh = mtod(mp, struct ether_vlan_header *);
	if (eh->evl_encap_proto == htons(ETHERTYPE_VLAN)) {
		etype = ntohs(eh->evl_proto);
		ehdrlen = ETHER_HDR_LEN + ETHER_VLAN_ENCAP_LEN;
	} else {
		etype = ntohs(eh->evl_encap_proto);
		ehdrlen = ETHER_HDR_LEN;
	}

	/* Set the ether header length */
	vlan_macip_lens |= ehdrlen << IXGBE_ADVTXD_MACLEN_SHIFT;

	if (*offload == FALSE)
		goto no_offloads;

		switch (etype) {
		case ETHERTYPE_IP:
			ip = (struct ip *)(mp->m_data + ehdrlen);
			ip_hlen = ip->ip_hl << 2;
			ipproto = ip->ip_p;
			type_tucmd_mlhl |= IXGBE_ADVTXD_TUCMD_IPV4;
			break;
		case ETHERTYPE_IPV6:
			ip6 = (struct ip6_hdr *)(mp->m_data + ehdrlen);
			ip_hlen = sizeof(struct ip6_hdr);
			/* XXX-BZ this will go badly in case of ext hdrs. */
			ipproto = ip6->ip6_nxt;
			type_tucmd_mlhl |= IXGBE_ADVTXD_TUCMD_IPV6;
			break;
		default:
			*offload = FALSE;
			break;
	}

	vlan_macip_lens |= ip_hlen;

	switch (ipproto) {
		case IPPROTO_TCP:
			if (mp->m_pkthdr.csum_flags & CSUM_TCP)
				type_tucmd_mlhl |= IXGBE_ADVTXD_TUCMD_L4T_TCP;
                        break;			

	        case IPPROTO_UDP:
			if (mp->m_pkthdr.csum_flags & CSUM_UDP)
				type_tucmd_mlhl |= IXGBE_ADVTXD_TUCMD_L4T_UDP;
			break;

#if __FreeBSD_version >= 800000
		case IPPROTO_SCTP:
			if (mp->m_pkthdr.csum_flags & CSUM_SCTP)
				type_tucmd_mlhl |= IXGBE_ADVTXD_TUCMD_L4T_SCTP;
			break;
#endif
		default:
			*offload = FALSE;
			break;
	}

	if (*offload) /* For the TX descriptor setup */
		*olinfo_status |= IXGBE_TXD_POPTS_TXSM << 8;

no_offloads:
	type_tucmd_mlhl |= IXGBE_ADVTXD_DCMD_DEXT | IXGBE_ADVTXD_DTYP_CTXT;

	/* Now copy bits into descriptor */
	TXD->vlan_macip_lens = htole32(vlan_macip_lens);
	TXD->type_tucmd_mlhl = htole32(type_tucmd_mlhl);
	TXD->seqnum_seed = htole32(0);
	TXD->mss_l4len_idx = htole32(0);

        return (0);
}

static int
ixgbe_isc_txd_encap(void *arg, if_pkt_info_t pi)
{
  struct adapter *sc       = arg;
  struct ix_queue *que     = sc->queues[pi->ipi_qsidx];
  struct tx_ring *txr      = que->txr;
  struct mbuf *m_head      = pi->ipi_m;
  struct ixgb_tx_buf *buf;
  int         nsegs        = pi->ipi_nsegs;
  bus_dma_segment_t *segs  = pi->ipi_segs;
  union ixgbe_adv_tx_desc *txd = NULL;

  int                     i, j, error;
  int                     first, last = 0;
  u16                     vtag = 0; 
  u32                     olinfo_status, cmd, off;

  cmd =  (IXGBE_ADVTXD_DTYP_DATA |
	    IXGBE_ADVTXD_DCMD_IFCS | IXGBE_ADVTXD_DCMD_DEXT);

  if (m_head->m_flags & M_VLANTAG)
		cmd |= IXGBE_ADVTXD_DCMD_VLE;
  
  i = first = pi->ipi_pidx;

  /*********************************************
   * Set up the appropriate offload context
   * this will consume the first descriptor
   *********************************************/
  error = ixgbe_tx_ctx_setup(txr, m_head, &cmd, &olinfo_status, first, &offload);
  if (error)
    return error; 
  
  if (offload)
    i++;

  for (j = 0; j < nsegs; j++) {
    bus_size_t seglen;
    bus_addr_t segaddr;

    txd = &txr->tx_base[i];
    seglen = segs[j].ds_len;
    segaddr = htole64(segs[j].ds_addr);

    txd->read.buffer_addr = segaddr;
    txd->read.cmd = htole32(txr->txd_cmd |
			    cmd_type_len |seglen);
    txd->read.olinfo_status = htole32(olinfo_status);

    if (++i == ixgbe_sctx->isc_ntxd)
      i = 0;
  }

  txd->read.cmd_type_len |=
    htole32(IXGBE_TXD_CMD_EOP | IXGBE_TXD_CMD_RS);

  /* Set the EOP descriptor that will be marked done */
  buf = &txr->tx_buffers[first];
  buf->eop = txd;
  ++txr->total_packets;
  pi->ipi_new_pidx = i; 
  
  return (0); 
}
  
static void
ixgbe_isc_txd_flush(void *arg, uint16_t txqid, uint32_t pidx)
{
  struct adapter *sc       = arg;
  struct ix_queue *que     = sc->queues[txqid];
  struct tx_ring *txr      = que->txr;
  
  IXGBE_WRITE_REG(&sc->hw, txr->tail, pidx);
}

static int
ixgbe_isc_txd_credits_update(void *arg, uint16_t txqid, uint32_t cidx)
{
  struct adapter   *sc = arg;
  struct ix_queue  *que = sc->queues[txqid];
  struct tx_ring   *txr = que->txr;
	
  u32			work, processed = 0;
  struct ixgbe_tx_buf	*buf;
  union ixgbe_adv_tx_desc *txd;
  int limit;
  
  limit = ixgbe_sctx->isc_ntxd - 1;
  /* Get work starting point */
  work = cidx;
  buf = &txr->tx_buffers[work];
  txd = &txr->tx_base[work];
  /* The distance to ring end */
  work -= ixgbe_sctx->isc_ntxd;
  
  do {
    union ixgbe_adv_tx_desc *eop= buf->eop;
    if (eop == NULL) /* No work */
      break;
    
    if ((eop->wb.status & IXGBE_TXD_STAT_DD) == 0)
      break;	/* I/O not complete */
    
    buf->eop = NULL; /* clear indicate processed */
    
    /* We clean the range if multi segment */
    while (txd != eop) {
      ++txd;
      ++buf;
      ++work;
      /* wrap the ring? */
      if (__predict_false(!work)) {
	work -= ixgbe_sctx->isc_ntxd;
	buf = txr->tx_buffers;
	txd = txr->tx_base;
      }
      buf->eop = NULL;
    }
    ++txr->packets;
    ++processed;
    
    /* Try the next packet */
    ++txd;
    ++buf;
    ++work;
    /* reset with a wrap */
    if (__predict_false(!work)) {
      work -= ixgbe_sctx->isc_ntxd;
      buf = txr->tx_buffers;
      txd = txr->tx_base;
    }
    prefetch(txd);
  } while (__predict_true(--limit));
  
  return (processed);
}

static void ixgbe_isc_rxd_refill(void *arg, uint16_t rxqid, uint8_t flid __unused,
				   uint32_t pidx, uint64_t *paddrs, caddr_t *vaddrs __unused, uint16_t count)
{
  struct adapter *sc       = arg;
  struct ix_queue *que     = sc->queues[rxqid];
  struct rx_ring *rxr      = que->rxr;

  int			i;
  uint32_t next_pidx;

  for (i = 0, next_pidx = pidx; i < count; i++) {
    rxr->rx_base[next_pidx].read.pkt_addr = htole64(paddrs[i]);
    if (++next_pidx == ixl_sctx->isc_nrxd)
      next_pidx = 0;
  }
}

static void ixgbe_isc_rxd_flush(void *arg, uint16_t rxqid, uint8_t flid __unused, uint32_t pidx)
{
  struct adapter *sc       = arg;
  struct ix_queue *que     = sc->queues[rxqid];
  struct rx_ring *rxr      = que->rxr;

  IXGBE_WRITE_REG(&sc->hw, rxr->tail, pidx);
}

static int ixgbe_isc_rxd_available(void *arg, uint16_t rxqid, uint32_t idx)
{
  struct adapter *sc       = arg;
  struct ix_queue *que     = sc->queues[rxqid];
  struct rx_ring *rxr      = que->rxr;
  union ixgbe_adv_rx_desc *rxd;
  u16                      pkt_info;
  u32                      staterr = 0; 
  int                      cnt, i;
  
  for (cnt = 0, i = idx; cnt < ixgbe_sctx->isc_nrxd) {
    rxd = &rxr->rx_base[i];
    staterr = le32toh(rxd->wb.upper.status_error);
    pkt_info = le16toh(rxd->wb.lower.lo_dword.hs_rss.pkt_info);

    if ((staterr & IXGBE_RXD_STAT_DD) == 0)
      break;
    cnt++; 
    
    if (++i == ixgbe_sctx->isc_nrxd)
      i = 0; 
  }
  
  return (cnt); 
}

/****************************************************************
 * Routine sends data which has been dma'ed into host memory
 * to upper layer. Initialize ri structure. 
 *
 * Returns 0 upon success, errno on failure
 ***************************************************************/

static int
ixgbe_isc_rxd_pkt_get(void *arg, if_rxd_info_t ri)
{
  struct adapter           *sc = arg;
  struct ix_queue          *que = sc->queues[ri->ri_qsidx];
  struct rx_ring           *rxr = &que->rxr;
  struct ifnet             *ifp = sc->ifp; 
  union ixgbe_adv_rx_desc  *rxd;
  u16                      pkt_info, len;
  u16                      vtag = 0; 
  u32                      ptype;
  u32                      staterr = 0; 
  bool                     eop;
  
  ri->iri_qidx = 0; 
  rxd = &rxr->rx_base[ri->iri_cidx];
  staterr = le32toh(rxd->wb.upper.status_error);
  pkt_info = le16toh(rxd->wb.lower.lo_dword.hs_rss.pkt_info);

   /* Error Checking then decrement count */
  MPASS ((staterr & IXGBE_RXD_STAT_DD) == 0);

    len = le16toh(rxd->wb.upper.length);
    ptype = le32toh(rxd->wb.lower.lo_dword.data) &
		IXGBE_RXDADV_PKTTYPE_MASK;
   
    ri->iri_len = len;
    rxr->rx_bytes += len;

    rxd->wb.upper.status_error = 0;
    eop = ((staterr & IXGBE_RXD_STAT_EOP) != 0);
    if (staterr & IXGBE_RXD_STAT_VP) {
	vtag = le16toh(rxd->wb.upper.vlan);
    } else {
      vtag = 0; 
    }
	
    /* Make sure bad packets are discarded */
    if (eop && (staterr & IXGBE_RXDADV_ERR_FRAME_ERR_MASK) != 0) {

        #if __FreeBSD_version >= 1100036
          if (IXGBE_IS_VF(adapter))
          if_inc_counter(ifp, IFCOUNTER_IERRORS, 1);
         #endif

         rxr->rx_discarded++;
	 return (EBADMSG);
    }

    /* Prefetch the next buffer */
     if (!eop) {
       ri->iri_next_offset = 1; 
     } else {
       rxr->rx_packets++;
       rxr->packets++;

       	if ((ifp->if_capenable & IFCAP_RXCSUM) != 0)
	  ixgbe_rx_checksum(ri, staterr, ptype);

#ifdef RSS
        ri->iri_flowid =
	   le32toh(rxd->wb.lower.hi_dword.rss);
	ri->iri_rsstype = ixgbe_determine_rsstype(pkt_info);
#else
	ri->iri_flowid = que->msix;
	ri->iri_rsstype = M_HASHTYPE_OPAQUE;
#endif	
       
        if (vtag) {
	  ri->iri_vtag = vtag;
	  ri->iri_flags |= M_VLANTAG;
	}
	ri->iri_next_offset = 0;
     }
     return (0); 
}

/********************************************************************
 *
 *  Parse the packet type to determine the appropriate hash
 *
 ******************************************************************/
static int 
ixgbe_determine_rsstype(u16 pkt_info)
{
     switch (pkt_info & IXGBE_RXDADV_RSSTYPE_MASK) {  
     case IXGBE_RXDADV_RSSTYPE_IPV4_TCP:
       return M_HASHTYPE_RSS_TCP_IPV4;
     case IXGBE_RXDADV_RSSTYPE_IPV4:
       return M_HASHTYPE_RSS_IPV4;
     case IXGBE_RXDADV_RSSTYPE_IPV6_TCP:
       return M_HASHTYPE_RSS_TCP_IPV6;
     case IXGBE_RXDADV_RSSTYPE_IPV6_EX:
       return M_HASHTYPE_RSS_IPV6_EX;
     case IXGBE_RXDADV_RSSTYPE_IPV6:
       return M_HASHTYPE_RSS_IPV6;
     case IXGBE_RXDADV_RSSTYPE_IPV6_TCP_EX:
       return M_HASHTYPE_RSS_TCP_IPV6_EX;
     case IXGBE_RXDADV_RSSTYPE_IPV4_UDP:
       return M_HASHTYPE_RSS_UDP_IPV4;
     case IXGBE_RXDADV_RSSTYPE_IPV6_UDP:
       return M_HASHTYPE_RSS_UDP_IPV6;
     case IXGBE_RXDADV_RSSTYPE_IPV6_UDP_EX:
       return M_HASHTYPE_RSS_UDP_IPV6_EX;
     default:
       return M_HASHTYPE_OPAQUE;
     }
}

/*********************************************************************
 *
 *  Verify that the hardware indicated that the checksum is valid.
 *  Inform the stack about the status of checksum so that stack
 *  doesn't spend time verifying the checksum.
 *
 *********************************************************************/
static void
ixgbe_rx_checksum(if_rxd_info_t ri, u32 staterr, u8 ptype)
{
  	u16	status = (u16) staterr;
	u8	errors = (u8) (staterr >> 24);
	bool	sctp = FALSE;

	if ((ptype & IXGBE_RXDADV_PKTTYPE_ETQF) == 0 &&
	    (ptype & IXGBE_RXDADV_PKTTYPE_SCTP) != 0)
		sctp = TRUE;

	if (status & IXGBE_RXD_STAT_IPCS) {
		if (!(errors & IXGBE_RXD_ERR_IPE)) {
			/* IP Checksum Good */
			ri->iri_csum_flags = CSUM_IP_CHECKED;
			ri->iri_csum_flags |= CSUM_IP_VALID;

		} else
			ri->iri_csum_flags = 0;
	}
	if (status & IXGBE_RXD_STAT_L4CS) {
		u64 type = (CSUM_DATA_VALID | CSUM_PSEUDO_HDR);
#if __FreeBSD_version >= 800000
		if (sctp)
			type = CSUM_SCTP_VALID;
#endif
		if (!(errors & IXGBE_RXD_ERR_TCPE)) {
			ri->iri_csum_flags |= type;
			if (!sctp)
				ri->iri_csum_data = htons(0xffff);
		} 
	}
}
