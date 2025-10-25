#ifndef LIB_ARINC653_INC_IPC_QUEUEING_H
#define LIB_ARINC653_INC_IPC_QUEUEING_H

#include "types.h"

#include <pthread.h>

/**
 * Queue structure
 */
typedef struct {
    char name[32];
    size_t msg_size;
    uint32_t capacity;
    PORT_DIRECTION dir;
    uint32_t head, tail, count;
    void *buffer;
    pthread_mutex_t mtx;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} queuing_port_t;

/**
 * Create a Queue
 * 
 * Initializes the struct, allocates the buffer, sets up mutex and condition variables.
 */
RETURN_CODE_TYPE create_queuing_port(queuing_port_t *qp, const char *name, size_t msg_size, uint32_t capacity, PORT_DIRECTION dir);

/**
 * Send data using Queue
 * 
 * Blocks if the queue is full.
 */
RETURN_CODE_TYPE send_queuing_message(queuing_port_t *qp, const void *msg, size_t len, SYSTEM_TIME_NS timeout_ns);

/**
 * Receive data using Queue
 * 
 * Blocks if the queue is empty.
 */
RETURN_CODE_TYPE receive_queuing_message(queuing_port_t *qp, void *msg, size_t *len, SYSTEM_TIME_NS timeout_ns);


#endif /* LIB_ARINC653_INC_IPC_QUEUEING_H */