#ifndef RUNNER_H
#define RUNNER_H

#include <stdbool.h>
#include "cpu_model.h"
#include "stimulus.h"
#include "tracer.h"

void run_simulation(cpu_t *cpu, stimulus_source_t *source, bool verbose, tracer_t *tracer);

#endif
