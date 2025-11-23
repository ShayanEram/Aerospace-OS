#ifndef LIB_ARINC653_INC_ERROR_H
#define LIB_ARINC653_INC_ERROR_H

#include "types.h"
#include "model.h"

typedef void (*error_callback_t)(partition_t*, const char* msg, RETURN_CODE_TYPE code);

/**
 * Associates a callback function with a given partition.
 */
void set_partition_error_handler(partition_t *p, error_callback_t cb);

/**
 * If the partition has a registered callback, it’s invoked.
 * Or a diagnostic line is printed to stderr.
 */
void report_error(partition_t *p, const char *msg, RETURN_CODE_TYPE code);


#endif /* LIB_ARINC653_INC_ERROR_H */