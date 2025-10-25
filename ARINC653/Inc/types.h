#ifndef LIB_ARINC653_INC_TYPES_H
#define LIB_ARINC653_INC_TYPES_H

#include <stdint.h>
#include <pthread.h>

// Global arinc653 Typedef
typedef uint32_t    RETURN_CODE_TYPE;
typedef uint64_t    SYSTEM_TIME_NS;
typedef int32_t     PROCESS_ID_TYPE;
typedef int32_t     PARTITION_ID_TYPE;

/**
 * Return codes in an ARINC 653‑style API.
 * - RC_OK → Operation succeeded.
 * - RC_INVALID → Invalid parameter or handle passed to the service.
 * - RC_NOT_AVAILABLE → Resource not available (e.g., port not ready, buffer empty).
 * - RC_TIMEOUT → Operation timed out (common in blocking receive calls).
 * - RC_NO_MEMORY → Allocation failed or insufficient memory for the request.
 * - RC_PORT_ERROR → Communication port error (misconfigured, corrupted, or unavailable).
 * - RC_SCHED_ERROR → Scheduling error (partition not scheduled, or service called at an invalid time).
 */
enum {
    RC_OK = 0,
    RC_INVALID,
    RC_NOT_AVAILABLE,
    RC_TIMEOUT,
    RC_NO_MEMORY,
    RC_PORT_ERROR,
    RC_SCHED_ERROR
};

/**
 * ports are the communication endpoints between partitions 
 * (or between a partition and the outside world). 
 * Each port has a direction:
 * - SOURCE → a port that sends messages
 * - DESTINATION → a port that receives messages
 */
typedef enum {
    DIR_SOURCE = 0,
    DIR_DESTINATION = 1
} PORT_DIRECTION; 

/**
 * Process priority list
 */
typedef enum{
    VERY_LOW  = 1,
    LOW       = 2,
    NORMAL    = 3,
    HIGH      = 4,
    VERY_HIGH = 5

} PRIORITY_TYPE;

/**
 * Process running or not
 */
typedef enum{
    FALSE,
    TRUE
} RUNNING_TYPE;

/**
 * Partition active or not
 */
typedef enum{
    FALSE,
    TRUE
} ACTIVE_TYPE;

#endif /* LIB_ARINC653_INC_TYPES_H */