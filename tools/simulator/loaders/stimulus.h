#ifndef STIMULUS_H
#define STIMULUS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t din1;
    uint8_t din2;
    uint8_t din3;
    uint8_t cmd;
    
    bool     has_expected;
    uint16_t expected_dout;
    bool     expected_error;
    bool     expected_zero;
} stimulus_t;

typedef struct stimulus_source stimulus_source_t;

struct stimulus_source {
    void *context; 
    bool (*next_stimulus)(stimulus_source_t *self, stimulus_t *out_stim);
    void (*close)(stimulus_source_t *self);
};

#endif