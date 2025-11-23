#include "process.h"

#include <linux/time.h>
#include <time.h>
#include <stdio.h>

/**
 * Returns current monotonic time in nanoseconds.
 * Used for scheduling and measuring execution time.
 */
static SYSTEM_TIME_NS mono_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    SYSTEM_TIME_NS mono_ns = (SYSTEM_TIME_NS)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    return mono_ns;
}

/**
 * This is the thread entry function for each process.
 * 
 * Wait for partition activation.
 * Period release check : If it woke up early, it sleeps until the next release time.
 * Execution budget measurement: Measures how long the process’s entry_point function took.
 * Advance to next release.
 */
static void* process_trampoline(void *arg) {
    proc_ctx_t *ctx = (proc_ctx_t*)arg;
    process_t *pr   = ctx->proc;
    partition_t *pt = ctx->part;

    SYSTEM_TIME_NS next_release = mono_ns();

    while(ctx->should_run)
    {
        partition_wait_active(pt);

        SYSTEM_TIME_NS now = mono_ns();
        if (now < next_release) {
            struct timespec ts = 
            { 
                .tv_sec = (next_release - now)/1000000000ULL,
                .tv_nsec = (next_release - now)%1000000000ULL
            };
            nanosleep(&ts, NULL);
        }

        SYSTEM_TIME_NS start = mono_ns();
        pr->entry_point();
        SYSTEM_TIME_NS elapsed = mono_ns() - start;

        if(elapsed > pr->timecap_ns)
        {
            // TODO: Error handler here
        }

        next_release += pr->period_ns;
    }
    return NULL;
}

RETURN_CODE_TYPE start_process(process_t *pr, partition_t *pt) {
    proc_ctx_t *ctx = calloc(1, sizeof(proc_ctx_t));

    if(!ctx){
        return RC_NO_MEMORY;
    }

    ctx->proc = pr;
    ctx->part = pt;
    ctx->should_run = 1;

    int rc = pthread_create(&pr->thread, NULL, process_trampoline, ctx);
    if(rc != 0){
        return RC_INVALID;
    }

    pr->running = 1;

    return RC_OK;
}

RETURN_CODE_TYPE stop_process(process_t *pr) {
    pr->running = 0;

    // TODO: close threads and free

    return RC_OK;
}

// TODO: priority to implement