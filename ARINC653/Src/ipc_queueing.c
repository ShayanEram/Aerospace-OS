#include "ipc_queueing.h"

#include <string.h>

RETURN_CODE_TYPE create_queuing_port(queuing_port_t *qp, const char *name, size_t msg_size, uint32_t capacity, PORT_DIRECTION dir){
    memset(qp, 0, sizeof(*qp));

    strncpy(qp->name, name, sizeof(qp->name) - 1);
    qp->msg_size = msg_size;
    qp->capacity = capacity;
    qp->dir = dir;
    qp->buffer = malloc(capacity * msg_size);
    pthread_mutex_init(&qp->mtx, NULL);
    pthread_cond_intit(&qp->not_empty, NULL);
    pthread_cond_init(&qp->not_full, NULL);

    return qp->buffer ? RC_OK : RC_NO_MEMORY;
}

RETURN_CODE_TYPE send_queuing_message(queuing_port_t *qp, const void *msg, size_t len, SYSTEM_TIME_NS timeout_ns){
    if(qp->dir != DIR_SOURCE || len != qp->msg_size){
        return RC_PORT_ERROR;
    }

    pthread_mutex_lock(&qp->mtx);
    while(qp->count == qp-> capacity){
        pthread_cond_wait(&qp->not_full, &qp->mtx);
    }
    memcpy((char*)qp->buffer + qp->tail * qp->msg_size, msg, len);
    qp->tail = (qp->tail + 1) % qp->capacity; 
    qp->count++;
    pthread_cond_signal(&qp->not_empty);
    pthread_mutex_unlock(&qp->mtx);

    return RC_OK;
}

RETURN_CODE_TYPE receive_queuing_message(queuing_port_t *qp, void *msg, size_t *len, SYSTEM_TIME_NS timeout_ns){
    if (qp->dir != DIR_DESTINATION) {
        return RC_PORT_ERROR;
    }

    pthread_mutex_lock(&qp->mtx);
    while (qp->count == 0) {
        pthread_cond_wait(&qp->not_empty, &qp->mtx);
    }
    memcpy(msg, (char*)qp->buffer + qp->head * qp->msg_size, qp->msg_size);
    qp->head = (qp->head + 1) % qp->capacity; 
    qp->count--;
    pthread_cond_signal(&qp->not_full);
    *len = qp->msg_size;
    pthread_mutex_unlock(&qp->mtx);

    return RC_OK;
}