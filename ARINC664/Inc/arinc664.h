#ifndef LIB_ARINC664_INC_ARINC664_H
#define LIB_ARINC664_INC_ARINC664_H

#include <stdint.h>
#include <stdbool.h>

#define AFDX_MAX_VL         256
#define AFDX_MAX_PAYLOAD    1471
#define AFDX_MAX_FRAME      1518
#define AFDX_REDUNDANT_NETS 2
#define AFDX_SEQ_MOD        16
#define AFDX_JITTER_US      500

typedef enum { 
    NET_A = 0, 
    NET_B = 1 
} afdx_net_t;

typedef struct {
    uint8_t  dest_mac[6];
    uint16_t ether_type;
    uint16_t vl_id;
    uint32_t bag_us;
    uint16_t max_l2_size;
    bool     policing_en;
    bool     redundancy_en;
} afdx_vl_cfg_t;

typedef struct {
    // shaping
    uint64_t next_eligible_ts_us;
    // sequence number
    uint8_t  tx_seq;              
    uint8_t  rx_expected_seq[AFDX_REDUNDANT_NETS];
    bool     rx_seq_init[AFDX_REDUNDANT_NETS];
} afdx_vl_state_t;

typedef struct {
    afdx_vl_cfg_t  cfg;
    afdx_vl_state_t st;
} afdx_vl_t;

typedef struct {
    afdx_vl_t vls[AFDX_MAX_VL];
    uint16_t  vl_count;
} afdx_end_system_t;

typedef struct {
    uint8_t  dst_mac[6];
    uint8_t  src_mac[6];
    uint16_t ethertype;
    uint8_t  payload[AFDX_MAX_PAYLOAD];
    uint16_t length;
    uint8_t  seq;
    uint16_t vl_id;
} afdx_frame_t;

typedef struct {
    void (*get_time_us)(uint64_t* now);
    int  (*tx_send)(afdx_net_t net, const uint8_t* frame, uint16_t len);
    int  (*rx_poll)(afdx_net_t net, uint8_t* frame_buf, uint16_t* len); // non-blocking
    void (*get_src_mac)(uint8_t mac[6]);
} afdx_hal_t;

typedef enum {
    AFDX_RX_OK = 0,
    AFDX_RX_DUPLICATE,
    AFDX_RX_OUT_OF_SEQUENCE,
    AFDX_RX_POLICED_OUT,
    AFDX_RX_INVALID
} afdx_rx_status_t;

typedef void (*afdx_rx_deliver_fn)(uint16_t vl_id, const uint8_t* payload, uint16_t len, afdx_rx_status_t st);

void afdx_init(afdx_end_system_t* es);
bool afdx_add_vl(afdx_end_system_t* es, const afdx_vl_cfg_t* cfg);
int afdx_tx_send(afdx_end_system_t* es, afdx_hal_t* hal, uint16_t vl_id, const uint8_t* payload, uint16_t payload_len);
int afdx_rx_poll(afdx_end_system_t* es, afdx_hal_t* hal, afdx_rx_deliver_fn deliver);

#endif /* LIB_ARINC664_INC_ARINC664_H */