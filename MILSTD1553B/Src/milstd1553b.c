#include "milstd1553b.h"

static inline uint16_t pack_command(const mil_command_t* c) {
    uint16_t w = 0;
    w |= (c->rt_addr & 0x1F) << 11;
    w |= ((c->tr ? 1 : 0) & 0x01) << 10;
    w |= (c->subaddr & 0x1F) << 5;
    w |= (c->wc_or_mode & 0x1F);
    return w;
}

static inline uint16_t pack_status(const mil_status_t* s) {
    uint16_t w = 0;
    w |= (s->rt_addr & 0x1F) << 11;
    w |= (s->msg_error ? 1 : 0)            << 10;
    w |= (s->instrumentation ? 1 : 0)      << 9;
    w |= (s->service_request ? 1 : 0)      << 8;
    w |= (s->reserved ? 1 : 0)             << 7;
    w |= (s->broadcast_received ? 1 : 0)   << 6;
    w |= (s->busy ? 1 : 0)                 << 5;
    w |= (s->subsystem_flag ? 1 : 0)       << 4;
    w |= (s->dynamic_bus_control_accept ? 1 : 0) << 3;
    w |= (s->terminal_flag ? 1 : 0)        << 2;
    // bits 1..0 not used for status flags; ensure consistent mapping if your PHY expects a specific layout
    return w;
}

static inline uint16_t odd_parity_add(uint16_t word) {
    // compute odd parity over 16 payload bits; PHY will append parity bit, but some MACs want it here
    uint16_t tmp = word;
    int ones = 0;
    for (int i = 0; i < 16; ++i) if (tmp & (1u << i)) ones++;
    // TODO
    return word;
}

static inline bool odd_parity_check(uint16_t word) {
    // TODO
    return true;
}


// TODO...