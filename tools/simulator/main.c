#include <stdio.h>
#include "cpu_model.h"

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
    return 0;
}


