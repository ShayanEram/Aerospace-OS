#ifndef LIB_ARINC653_INC_IPC_SAMPLING_H
#define LIB_ARINC653_INC_IPC_SAMPLING_H

#include "types.h"

#include <pthread.h>

/**
 * Only the latest message is visible — if you overwrite it, the old one is gone.
 * Each message has an age (time since it was written), so the receiver can decide if it’s still valid.
 */
typedef struct {
    char name[32];
    size_t msg_size;
    PORT_DIRECTION dir;
    pthread_mutex_t mtx;
    SYSTEM_TIME_NS timestamp_ns;
    void *buffer;
} sampling_port_t;

/**
 * Initializes the struct, allocates a fixed‑size buffer, sets direction.
 * Mirrors ARINC 653’s CREATE_SAMPLING_PORT service.
 */
RETURN_CODE_TYPE create_sampling_port(sampling_port_t *sp, const char *name, size_t msg_size, PORT_DIRECTION dir);

/**
 * Only valid if the port is a SOURCE.
 * Overwrites any previous message (sampling semantics).
 */
RETURN_CODE_TYPE write_sampling_message(sampling_port_t *sp, const void *msg, size_t len);

/**
 * Only valid if the port is a DESTINATION.
 * Receiver can decide if the message is still “fresh” enough.
 */
RETURN_CODE_TYPE read_sampling_message(sampling_port_t *sp, void *msg, size_t *len, SYSTEM_TIME_NS *age_ns);

#endif /* LIB_ARINC653_INC_IPC_SAMPLING_H */