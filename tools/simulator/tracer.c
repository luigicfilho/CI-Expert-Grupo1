#include "tracer.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE *file;
} tracer_csv_ctx_t;

static void tracer_csv_log_cycle(tracer_t *self, uint32_t cycle, int instr_idx, int sub_cycle, const cpu_t *cpu) {
    tracer_csv_ctx_t *ctx = (tracer_csv_ctx_t *)self->context;

    fprintf(ctx->file, "%u,%d,%d,%d,%d,0x%02X,%u,%u,%u,0x%02X,0x%02X,0x%04X,%d,%d\n",
            cycle,
            instr_idx,
            sub_cycle,
            cpu->state,
            cpu->cpu_rdy ? 1 : 0,
            cpu->cmd_in,
            cpu->din[0],
            cpu->din[1],
            cpu->din[2],
            cpu->reg_a,
            cpu->reg_b,
            cpu->reg_out,
            cpu->flag_zero ? 1 : 0,
            cpu->flag_error ? 1 : 0);
}

static void tracer_csv_close(tracer_t *self) {
    tracer_csv_ctx_t *ctx = (tracer_csv_ctx_t *)self->context;
    if (ctx->file) {
        fclose(ctx->file);
    }
    free(ctx);
    free(self);
}

tracer_t* tracer_open_csv(const char *filepath) {
    FILE *f = fopen(filepath, "w");
    if (!f) {
        return NULL;
    }

    tracer_csv_ctx_t *ctx = malloc(sizeof(tracer_csv_ctx_t));
    ctx->file = f;

    // Cabecalho CSV padronizado para comparacao de waveforms / DV
    fprintf(f, "cycle,instr,sub_cycle,state,cpu_rdy,cmd_in,din1,din2,din3,reg_a,reg_b,reg_out,zero,error\n");

    tracer_t *t = malloc(sizeof(tracer_t));
    t->context = ctx;
    t->log_cycle = tracer_csv_log_cycle;
    t->close = tracer_csv_close;
    return t;
}
