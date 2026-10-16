#include "runner.h"
#include "stimulus.h"
#include "cpu_model.h"
#include "tracer.h"
#include <stdio.h>

int run_simulation(cpu_t *cpu, stimulus_source_t *source, bool verbose, tracer_t *tracer) {
    stimulus_t stim;
    int instr_idx = 0;
    int fail_count = 0;
    uint32_t total_cycles = 0;
    printf(" Iniciando Simulacao...\n");
    
    while (source->next_stimulus(source, &stim)) {
        cpu->din[0] = stim.din1;
        cpu->din[1] = stim.din2;
        cpu->din[2] = stim.din3;
        cpu->cmd_in = stim.cmd;

        for (int cycle = 0; cycle < 3; cycle++) {
            cpu_step(cpu);
            total_cycles++;

            if (tracer) {
                tracer->log_cycle(tracer, total_cycles, instr_idx, cycle, cpu);
            }

            if (verbose) {
                printf("[Instr %d | Ciclo %d] State=%d RDY=%d RegOut=0x%04X Zero=%d Err=%d\n",
                       instr_idx, cycle, cpu->state, cpu->cpu_rdy, 
                       cpu->reg_out, cpu->flag_zero, cpu->flag_error);
            }
        }

        if (stim.has_expected) {
            bool match = (cpu->reg_out == stim.expected_dout) &&
                        (cpu->flag_error == stim.expected_error) &&
                        (cpu->flag_zero == stim.expected_zero);
                        
            if (match) {
                printf("  -> [PASS] Instr %d OK\n", instr_idx);
            } else {
                fail_count++;
                printf("  -> [FAIL] Instr %d: esperado (out=0x%04X, err=%d, zero=%d), "
                    "obtido (out=0x%04X, err=%d, zero=%d)\n",
                    instr_idx,
                    stim.expected_dout, stim.expected_error, stim.expected_zero,
                    cpu->reg_out, cpu->flag_error, cpu->flag_zero);
            }
        }
        instr_idx++;
    }
    
    printf(" Simulacao Finalizada: %d instrucoes executadas (%u ciclos)\n\n", instr_idx, total_cycles);

    printf("Estado Final da Memoria Interna \n");
    for (int i = 0; i < 8; i++) {
        printf("  Mem[%d] = 0x%04X (%5u)\n", i, cpu->memory[i], cpu->memory[i]);
    }
    printf("---\n");

    return fail_count;
}