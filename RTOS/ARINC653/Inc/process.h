#ifndef LIB_ARINC653_INC_PROCESS_H
#define LIB_ARINC653_INC_PROCESS_H

#include "model.h"
#include "types.h"
#include "process.h"
#include "partition.h"

#include <stdatomic.h>

/**
 * A small context object passed to each thread.
 */
typedef struct{
    process_t   *proc;
    partition_t *part;
    atomic_int  should_run;
} proc_ctx_t;

/**
 * Start a process: Spawn a POSIX thread 
 */
RETURN_CODE_TYPE start_process(process_t *pr, partition_t *pt);

/**
 * Stop process
 */
RETURN_CODE_TYPE stop_process(process_t *pr);

#endif /* LIB_ARINC653_INC_PROCESS_H */