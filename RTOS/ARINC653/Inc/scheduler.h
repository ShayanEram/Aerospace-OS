#ifndef LIB_ARINC653_INC_SCHEDULER_H
#define LIB_ARINC653_INC_SCHEDULER_H

#include "model.h"
#include "partition.h"

/**
 * Implement the major frame scheduler. 
 * The schedule is static and cyclic: every major frame repeats the same sequence of partition activations.
 * This guarantees temporal isolation: no partition can overrun into another’s time, which is critical for safety certification.
 * 
 * - Align to the start of the current major frame (frame_start).
 * - Iterates through all partitions (sys->parts).
 * - For each partition:
 *      - wait for offset, activate partion, wait window time, deactivate partition.
 */
void run_scheduler(system_t *sys);

#endif /* LIB_ARINC653_INC_SCHEDULER_H */