#ifndef LIB_ARINC653_INC_PARTITION_H
#define LIB_ARINC653_INC_PARTITION_H

#include "model.h"

/**
 * Initialization of a partition
 */
void partition_init(partition_t *p, const char *name, SYSTEM_TIME_NS window_ns, SYSTEM_TIME_NS offset_ns);

/**
 * Activation of a process
 */
void partition_set_active(partition_t *p, ACTIVE_TYPE active);

/**
 * Cooperative wait
 */
void partition_wait_active(partition_t *p);

#endif /* LIB_ARINC653_INC_PARTITION_H */