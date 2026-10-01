#include <stdio.h>
#include "cpu_model.h"

#define CMD(muxA, muxB, op) (((muxA) << 5) | ((muxB) << 3) | (op))
void rodar_instrucao(cpu_t *cpu, uint8_t cmd);

int main() {
    cpu_t cpu = {0};
    bool zero = false, error = false;

    uint16_t resultado;
    printf("Simulador\n");
    resultado = alu_executar(OP_ADD, 3, 4, false,  &zero, &error);
    printf("ADD 3+4 = %u, zero=%d, error=%d\n", resultado, zero, error);
    resultado = alu_executar(OP_MUL, 200, 200, false, &zero, &error);
    printf("MUL 200*200 = %u, zero=%d, error=%d\n", resultado, zero, error);
    resultado = alu_executar(OP_DIV, 10, 0, false, &zero, &error);
    printf("DIV 10/0 = %u, zero=%d, error=%d\n", resultado, zero, error);
    cpu.reg_out = 0xAB12;
    cpu.cmd_in = 0x78;
    decode_ciclo_fetch(&cpu);
    printf("Decode Test, regA=%02X, regB=%02X\n", cpu.reg_a, cpu.reg_b);

    cpu_step(&cpu);
    cpu.din[0] = 10;
    cpu.din[1] = 32;
    rodar_instrucao(&cpu, CMD(0, 1, OP_ADD)); 
    cpu.din[2] = 2;
    rodar_instrucao(&cpu, CMD(2, 0, OP_STORE));
    rodar_instrucao(&cpu, CMD(2, 0, OP_LOAD));
    for (int i = 0; i < sizeof(cpu.memory) / sizeof(cpu.memory[0]); i++){
        printf("CPU memory pos=%d value=%d\n", i, cpu.memory[i]);
    }
    return 0;
}


void rodar_instrucao(cpu_t *cpu, uint8_t cmd) {
    cpu->cmd_in = cmd;
    for (int i = 0; i < 3; i++) {
        cpu_step(cpu);
        printf("CPU state=%d rdy=%d reg_out=0x%04X zero=%d error=%d\n",
               cpu->state, cpu->cpu_rdy, cpu->reg_out,
               cpu->flag_zero, cpu->flag_error);
    }
}