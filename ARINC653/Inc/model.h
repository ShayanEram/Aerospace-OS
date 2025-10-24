#ifndef LIB_ARINC653_INC_MODEL_H
#define LIB_ARINC653_INC_MODEL_H

#include "types.h"

/**
 * process_t: Represents a process (or thread) inside a partition.
 * 
 * - name[32]: identifier for debugging/logging.
 * - period_ns / timecap_ns: timing attributes (periodicity and execution budget).
 * - priority: scheduling priority within the partition.
 * - entry_point: function pointer to the process body.
 * - Runtime fields: pid, pthread_t, running → simulate ARINC 653 processes on top of POSIX threads.
 */
typedef struct{
    char name[32];
    SYSTEM_TIME_NS period_ns;
    SYSTEM_TIME_NS timecap_ns;
    PRIORITY_TYPE priority;
    void (*entry_point)(void);
    PROCESS_ID_TYPE pid;
    pthread_t thread;
    RUNNING_TYPE running;
} process_t;


/**
 * partition_t: Represents a partition (the ARINC 653 abstraction that groups processes and resources).
 * 
 * - window_ns: time slice allocated to this partition in the major frame.
 * - offset_ns: when in the major frame this partition’s window starts.
 * - num_processes / procs: array of processes belonging to this partition.
 * - Runtime fields: id, pthread_mutex_t, pthread_cond_t, active → simulate partition activation/deactivation and synchronization.
 */
typedef struct{
    

} partition_t;

/**
 * system_t: Represents the whole system schedule.
 * 
 * - major_frame_ns: the length of the major frame (the repeating schedule cycle).
 * - num_partitions / parts: array of partitions scheduled within the major frame.
 */
typedef struct{

} system_t;



#endif /* LIB_ARINC653_INC_MODEL_H */