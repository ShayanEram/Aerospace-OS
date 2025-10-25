#ifndef LIB_MILSTD1553B_INC_MILSTD1553B_H
#define LIB_MILSTD1553B_INC_MILSTD1553B_H

#include <stdint.h>
#include <stdbool.h>

#define MIL1553_MAX_DATA_WORDS   32     // typical max per message (spec allows up to 32)
#define MIL1553_MAX_RT           31     // RT addresses 0..30 (31 reserved or monitor)
#define MIL1553_SUBADDR_DATA     0..30  // data subaddresses; 31 is Mode Code
#define MIL1553_WORD_BITS        16

typedef enum { 
    MIL_DIR_RT_TO_BC = 1, 
    MIL_DIR_BC_TO_RT = 0 
} mil_dir_t;

typedef enum { 
    MIL_WORD_COMMAND, 
    MIL_WORD_STATUS, 
    MIL_WORD_DATA 
} mil_word_type_t;

typedef enum {
    MIL_MODE_DYNAMIC_BUS_CONTROL       = 0x00,
    MIL_MODE_SYNCHRONIZE               = 0x01,
    MIL_MODE_TRANSMITTER_SHUTDOWN      = 0x02,
    MIL_MODE_TRANSMITTER_SHUTDOWN_OFF  = 0x03,
    MIL_MODE_RESET_REMOTE_TERMINAL     = 0x04,
    MIL_MODE_TRANSMITTER_OVERRIDE      = 0x05,
    MIL_MODE_INHIBIT_TERMINAL_FLAG     = 0x06,
    MIL_MODE_INHIBIT_TERMINAL_FLAG_OFF = 0x07,
    MIL_MODE_BROADCAST_SYNC            = 0x11,
    // ...
} mil_mode_code_t;

typedef struct {
    uint8_t  rt_addr;     // 0..30
    mil_dir_t tr;         // 0 BC->RT, 1 RT->BC
    uint8_t  subaddr;     // 0..30 data; 31 indicates mode code
    uint8_t  wc_or_mode;  // 1..32 words or mode code value when subaddr==31
} mil_command_t;

typedef struct {
    uint8_t  rt_addr;     // echo RT address
    bool     msg_error;
    bool     instrumentation;
    bool     service_request;
    bool     reserved;            // often 0
    bool     broadcast_received;
    bool     busy;
    bool     subsystem_flag;
    bool     dynamic_bus_control_accept;
    bool     terminal_flag;
} mil_status_t;

typedef struct {
    mil_command_t cmd;
    uint16_t      data[MIL1553_MAX_DATA_WORDS];
    uint8_t       data_len; // in words
} mil_message_t;

#endif /* LIB_MILSTD1553B_INC_MILSTD1553B_H */