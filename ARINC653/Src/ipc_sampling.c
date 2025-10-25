#include "ipc_sampling.h"

#include <string.h>
#include <linux/time.h>
#include <time.h>

RETURN_CODE_TYPE create_sampling_port(sampling_port_t *sp, const char *name, size_t msg_size, PORT_DIRECTION dir) {
    memset(sp, 0, sizeof(*sp));

    strncpy(sp->name, name, sizeof(sp->name) - 1);
    sp->msg_size = msg_size;
    sp->buffer = malloc(msg_size);
    pthread_mutex_init(&sp->mtx, NULL);

    return sp->buffer ? RC_OK : RC_NO_MEMORY; 
}

RETURN_CODE_TYPE write_sampling_message(sampling_port_t *sp, const void *msg, size_t len){
    if(sp->dir != DIR_SOURCE || len > sp->msg_size){
        return RC_PORT_ERROR;
    }

    pthread_mutex_lock(&sp->mtx);
    memcpy(sp->buffer, msg, len);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    sp->timestamp_ns = (SYSTEM_TIME_NS)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    pthread_mutex_unlock(&sp->mtx);

    return RC_OK;
}

RETURN_CODE_TYPE read_sampling_message(sampling_port_t *sp, void *msg, size_t *len, SYSTEM_TIME_NS *age_ns) {
    if(sp->dir != DIR_DESTINATION) {
        return RC_PORT_ERROR;
    }

    pthread_mutex_lock(&sp->mtx);
    memcpy(msg, sp->buffer, sp->msg_size);
    *len = sp->msg_size;
    struct timespec ts; 
    clock_gettime(CLOCK_MONOTONIC, &ts);
    *age_ns = ((SYSTEM_TIME_NS)ts.tv_sec * 1000000000ULL + ts.tv_nsec) - sp->timestamp_ns;
    pthread_mutex_unlock(&sp->mtx);

    return RC_OK;
}