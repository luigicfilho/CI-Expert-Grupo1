#ifndef TRACER_H
#define TRACER_H

#include <stdint.h>
#include "cpu_model.h"

typedef struct tracer tracer_t;

struct tracer {
    void *context;
    void (*log_cycle)(tracer_t *self, uint32_t cycle, int instr_idx, int sub_cycle, const cpu_t *cpu);
    void (*close)(tracer_t *self);
};

tracer_t* tracer_open_csv(const char *filepath);

#endif
