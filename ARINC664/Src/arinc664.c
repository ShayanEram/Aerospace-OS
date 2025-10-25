#include "arinc664.h"

#include <string.h>

static inline uint16_t min_u16(uint16_t a, uint16_t b) { 
    return a < b ? a : b; 
}

void afdx_init(afdx_end_system_t* es) {
    memset(es, 0, sizeof(*es));
}

bool afdx_add_vl(afdx_end_system_t* es, const afdx_vl_cfg_t* cfg) {
    // if (es->vl_count >= AFDX_MAX_VL) return false;
    // afdx_vl_t* vl = &es->vls[es->vl_count++];
    // vl->cfg = *cfg;
    // vl->st.next_eligible_ts_us = 0;
    // vl->st.tx_seq = 0;
    // for (int n = 0; n < AFDX_REDUNDANT_NETS; ++n) {
    //     vl->st.rx_seq_init[n] = false;
    //     vl->st.rx_expected_seq[n] = 0;
    // }
    return true;
}

static void build_eth_frame(afdx_frame_t* fr, const afdx_vl_cfg_t* cfg, const uint8_t src_mac[6], const uint8_t* payload, uint16_t payload_len, uint8_t seq) {
    // memcpy(fr->dst_mac, cfg->dest_mac, 6);
    // memcpy(fr->src_mac, src_mac, 6);
    // fr->ethertype = cfg->ether_type;
    // fr->seq = seq;
    // fr->length = (uint16_t)(14 + payload_len); // 14-byte Ethernet header, no FCS here (MAC adds)
    // fr->vl_id = cfg->vl_id;
    // memcpy(fr->payload, payload, payload_len);
}

static bool policing_check(const afdx_vl_cfg_t* cfg, uint16_t frame_len, uint64_t ts_us, uint64_t eligible_us) {
    // if (!cfg->policing_en) return true;
    // if (frame_len > cfg->max_l2_size) return false;
    // // allow JITTER window around BAG schedule
    // if (ts_us + AFDX_JITTER_US < eligible_us) return false; // too early beyond jitter
    return true;
}

int afdx_tx_send(afdx_end_system_t* es, afdx_hal_t* hal, uint16_t vl_id, const uint8_t* payload, uint16_t payload_len)
{
    // // find VL
    // afdx_vl_t* vl = NULL;
    // for (uint16_t i = 0; i < es->vl_count; ++i) {
    //     if (es->vls[i].cfg.vl_id == vl_id) { vl = &es->vls[i]; break; }
    // }
    // if (!vl) return -1;

    // uint8_t src_mac[6];
    // hal->get_src_mac(src_mac);

    // uint64_t now; hal->get_time_us(&now);
    // uint64_t eligible = vl->st.next_eligible_ts_us;

    // // frame build
    // afdx_frame_t fr;
    // uint16_t pl = min_u16(payload_len, AFDX_MAX_PAYLOAD);
    // build_eth_frame(&fr, &vl->cfg, src_mac, payload, pl, vl->st.tx_seq);

    // // policing and shaping
    // if (!policing_check(&vl->cfg, fr.length, now, eligible)) return -2;
    // if (now < eligible) return -3; // shaping: not yet eligible

    // // redundancy: send on A and B if enabled
    // int rcA = 0, rcB = 0;
    // rcA = hal->tx_send(NET_A, (uint8_t*)&fr, fr.length);
    // if (vl->cfg.redundancy_en) {
    //     rcB = hal->tx_send(NET_B, (uint8_t*)&fr, fr.length);
    // }

    // // advance BAG slot and sequence number modulo-16
    // vl->st.tx_seq = (uint8_t)((vl->st.tx_seq + 1) % AFDX_SEQ_MOD);
    // vl->st.next_eligible_ts_us = now + vl->cfg.bag_us;

    // // success if at least one network accepted
    // if (rcA < 0 && rcB < 0) return -4;
    return 0;
}

static inline bool seq_is_next(uint8_t expected, uint8_t incoming) {
    return ((expected + 1) % AFDX_SEQ_MOD) == incoming;
}

static afdx_rx_status_t validate_and_update_rx(afdx_vl_t* vl, afdx_net_t net, const afdx_frame_t* fr, uint64_t now)
{
    // // policing on RX: size check
    // if (vl->cfg.policing_en && fr->length > vl->cfg.max_l2_size) {
    //     return AFDX_RX_POLICED_OUT;
    // }

    // // initialize expected seq on first packet per network
    // if (!vl->st.rx_seq_init[net]) {
    //     vl->st.rx_expected_seq[net] = fr->seq;
    //     vl->st.rx_seq_init[net] = true;
    //     return AFDX_RX_OK;
    // }

    // uint8_t expected = vl->st.rx_expected_seq[net];

    // if (fr->seq == expected) {
    //     // duplicate (from other redundant path or retransmit)
    //     return AFDX_RX_DUPLICATE;
    // } else if (seq_is_next(expected, fr->seq)) {
    //     vl->st.rx_expected_seq[net] = fr->seq;
    //     return AFDX_RX_OK;
    // } else {
    //     // out-of-sequence; in AFDX this typically indicates loss or misordering beyond allowed
    //     // update expected to incoming (choice depends on system policy)
    //     vl->st.rx_expected_seq[net] = fr->seq;
    //     return AFDX_RX_OUT_OF_SEQUENCE;
    // }
}

int afdx_rx_poll(afdx_end_system_t* es, afdx_hal_t* hal, afdx_rx_deliver_fn deliver)
{
    uint8_t buf[AFDX_MAX_FRAME];
    uint16_t len = 0;
    uint64_t now; hal->get_time_us(&now);

    // for (int n = 0; n < AFDX_REDUNDANT_NETS; ++n) {
    //     afdx_net_t net = (afdx_net_t)n;
    //     while (hal->rx_poll(net, buf, &len) == 0 && len > 0) {
    //         // parse minimal frame view
    //         const afdx_frame_t* fr = (const afdx_frame_t*)buf;

    //         // map to VL by dest MAC + EtherType or internal table (here we use vl_id serialized)
    //         afdx_vl_t* vl = NULL;
    //         for (uint16_t i = 0; i < es->vl_count; ++i) {
    //             if (es->vls[i].cfg.vl_id == fr->vl_id) { vl = &es->vls[i]; break; }
    //         }
    //         if (!vl) continue;

    //         afdx_rx_status_t st = validate_and_update_rx(vl, net, fr, now);
    //         if (deliver) {
    //             deliver(vl->cfg.vl_id, fr->payload, (uint16_t)(fr->length - 14), st);
    //         }
    //         len = 0; // reset before next poll
    //     }
    // }
    return 0;
}