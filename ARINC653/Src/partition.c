#include "partition.h"

#include <string.h>

void partition_init(partition_t *p, const char *name, SYSTEM_TIME_NS window_ns, SYSTEM_TIME_NS offset_ns){
    memset(p, 0, sizeof(*p));

    strncpy(p->name, name, sizeof(p->name) - 1);
    p->window_ns = window_ns;
    p->offset_ns = offset_ns;
    pthread_mutex_init(&p->mtx, NULL);
    pthread_cond_init(&p->cv, NULL);
    p->active = FALSE;
}

void partition_set_active(partition_t *p, ACTIVE_TYPE active){
    pthread_mutex_lock(&p->mtx);
    p->active = active;
    if(active = TRUE) {
        pthread_cond_broadcaset(&p->mtx);
    }
    pthread_mutex_unlock(&p->mtx);
}

void partition_wait_active(partition_t *p){
    pthread_mutex_lock(&p->mtx);
    while(p->active = FALSE){
        pthread_cond_wait(&p->cv, &p->mtx);
    }
    pthread_mutex_unlock(&p->mtx);
}