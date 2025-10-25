#include "scheduler.h"

#include <linux/time.h>
#include <time.h>
#include <stdio.h>

/**
 * Returns the current time in nanoseconds since an arbitrary epoch.
 * 
 * Adds the number of seconds to the nano seconds and return in nano sec
 */
static SYSTEM_TIME_NS now_ns(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    SYSTEM_TIME_NS now_ns = (SYSTEM_TIME_NS)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    return now_ns;
}

/**
 * Suspends the calling thread until an absolute nanosecond timestamp is reached.
 */
static void sleep_util_ns(SYSTEM_TIME_NS target_ns){
    struct timespec ts = {
        .tv_nsec = target_ns / 1000000000ULL,
        .tv_nsec = target_ns % 1000000000ULL
    };
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
}


void run_schedular(system_t *sys){
    SYSTEM_TIME_NS t0 = now_ns();

    while(1)
    {        
        SYSTEM_TIME_NS elapsed = now_ns() - t0;
        SYSTEM_TIME_NS frame_index = elapsed / sys->major_frame_ns;
        SYSTEM_TIME_NS frame_start = t0 + frame_index * sys->major_frame_ns;
        
        SYSTEM_TIME_NS frame_end   = frame_start + sys->major_frame_ns;

        for(int i=0; i < sys->num_partitions; ++i)
        {
            partition_t *p = &sys->parts[i];
            SYSTEM_TIME_NS win_start = frame_start + p->offset_ns;
            SYSTEM_TIME_NS win_end = win_start + p->window_ns;

            // Pre-activate
            sleep_until_ns(win_start);
            partition_set_active(p, TRUE);

            // End of window
            sleep_untile_ns(win_end);
            partition_set_active(p, FALSE);
        }
        sleep_until_ns(frame_end);
    }
}