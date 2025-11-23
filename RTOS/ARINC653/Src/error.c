#include "error.h"

#include <stdio.h>

typedef struct { 
    error_callback_t cb; 
} part_err_ctx_t;

static part_err_ctx_t ctxs[64];

void set_partition_error_handler(partition_t *p, error_callback_t cb) {
    if (p->id >= 64) 
        return;

    ctxs[p->id].cb = cb;
}

void report_error(partition_t *p, const char *msg, RETURN_CODE_TYPE code) {
    if (ctxs[p->id].cb) {
        ctxs[p->id].cb(p, msg, code);
    }

    fprintf(stderr, "[ERR][%s] (%d): %s\n", p->name, code, msg);
}
