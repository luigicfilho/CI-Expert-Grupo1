#include "mem_loader.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE *file;
    int line_count;
} mem_file_ctx_t;

static bool mem_next(stimulus_source_t *self, stimulus_t *out_stim) {
    mem_file_ctx_t *ctx = (mem_file_ctx_t *)self->context;
    char linha[256];

    while (fgets(linha, sizeof(linha), ctx->file)) {

        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == ';' || linha[0] == '#' || (linha[0] == '/' && linha[1] == '/')) {
            continue;
        }

        if (linha[0] == '@') {
            continue;
        }

        unsigned int cmd;

        if (sscanf(linha, "%x", &cmd) == 1) {
            out_stim->din1 = 0;
            out_stim->din2 = 0;
            out_stim->din3 = 0;
            out_stim->cmd  = (uint8_t)cmd;
            out_stim->has_expected = false;
            ctx->line_count++;

            return true;
        }
    }

    return false;
}

static void mem_close(stimulus_source_t *self) {
    mem_file_ctx_t *ctx = (mem_file_ctx_t *)self->context;

    if (ctx->file) fclose(ctx->file);

    free(ctx);
    free(self);
}

stimulus_source_t* mem_loader_open(const char *filepath) {
    FILE *f = fopen(filepath, "r");

    if (!f) return NULL;
    
    mem_file_ctx_t *ctx = malloc(sizeof(mem_file_ctx_t));
    ctx->file = f;
    ctx->line_count = 0;
    stimulus_source_t *src = malloc(sizeof(stimulus_source_t));
    src->context = ctx;
    src->next_stimulus = mem_next;
    src->close = mem_close;
    return src;
}
