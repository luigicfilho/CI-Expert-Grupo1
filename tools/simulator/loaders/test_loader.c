#include "test_loader.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE *file;
    int line_count;
} test_file_ctx_t;

static bool test_next(stimulus_source_t *self, stimulus_t *out_stim) {
    test_file_ctx_t *ctx = (test_file_ctx_t *)self->context;
    char linha[256];
    
    while (fgets(linha, sizeof(linha), ctx->file)) {

        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == ';' || linha[0] == '#') {
            continue;
        }

        unsigned int d1, d2, d3, cmd, exp_dout, exp_err, exp_zero;

        int lidos = sscanf(linha, "%u %u %u %u %x %u %u", 
                           &d1, &d2, &d3, &cmd, 
                           &exp_dout, &exp_err, &exp_zero);
                           
        if (lidos >= 4) {
            out_stim->din1 = (uint8_t)d1;
            out_stim->din2 = (uint8_t)d2;
            out_stim->din3 = (uint8_t)d3;
            out_stim->cmd  = (uint8_t)cmd;
            if (lidos >= 7) {
                out_stim->has_expected   = true;
                out_stim->expected_dout  = (uint16_t)exp_dout;
                out_stim->expected_error = (exp_err != 0);
                out_stim->expected_zero  = (exp_zero != 0);
            } else {
                out_stim->has_expected = false;
            }
            ctx->line_count++;
            return true;
        }
    }
    return false;
}

static void test_close(stimulus_source_t *self) {
    test_file_ctx_t *ctx = (test_file_ctx_t *)self->context;
    if (ctx->file) fclose(ctx->file);
    free(ctx);
    free(self);
}

stimulus_source_t* test_loader_open(const char *filepath) {
    FILE *f = fopen(filepath, "r");

    if (!f) return NULL;
    
    test_file_ctx_t *ctx = malloc(sizeof(test_file_ctx_t));
    ctx->file = f;
    ctx->line_count = 0;
    stimulus_source_t *src = malloc(sizeof(stimulus_source_t));
    src->context = ctx;
    src->next_stimulus = test_next;
    src->close = test_close;
    return src;
}