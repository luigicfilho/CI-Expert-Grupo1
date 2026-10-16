#include "stim_loader.h"
#include "stimulus.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE *file;
    int line_count;
} stim_file_ctx_t;

static bool stim_next(stimulus_source_t *self, stimulus_t *out_stim) {
    stim_file_ctx_t *ctx = (stim_file_ctx_t *)self->context;
    unsigned int d1, d2, d3, cmd;
    
    if (fscanf(ctx->file, "%u %u %u %u", &d1, &d2, &d3, &cmd) == 4) {
        out_stim->din1 = (uint8_t)d1;
        out_stim->din2 = (uint8_t)d2;
        out_stim->din3 = (uint8_t)d3;
        out_stim->cmd  = (uint8_t)cmd;
        out_stim->has_expected = false;
        ctx->line_count++;
        return true;
    }

    return false;
}

static void stim_close(stimulus_source_t *self) {
    stim_file_ctx_t *ctx = (stim_file_ctx_t *)self->context;

    if (ctx->file) fclose(ctx->file);

    free(ctx);
    free(self);
}

stimulus_source_t* stim_loader_open(const char *filepath) {
    FILE *f = fopen(filepath, "r");

    if (!f) return NULL;
    
    stim_file_ctx_t *ctx = malloc(sizeof(stim_file_ctx_t));
    ctx->file = f;
    ctx->line_count = 0;
    stimulus_source_t *src = malloc(sizeof(stimulus_source_t));
    src->context = ctx;
    src->next_stimulus = stim_next;
    src->close = stim_close;
    return src;
}