#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include <stdint.h>
#include <stdbool.h>

#define WIDTH 8

typedef enum {
    RST = 0,        // 2'b00
    FETCH_DECODE,   // 2'b01
    EXECUTE,        // 2'b10
    STORE           // 2'b11
} state_t;

typedef enum {
    OP_ADD = 0,  // 000
    OP_SUB,      // 001
    OP_MUL,      // 010
    OP_DIV,      // 011
    OP_NOP,      // 100
    OP_LOAD,     // 101
    OP_STORE,    // 110
    OP_NOP2 = 7  // 111 
} opcode_t;

typedef struct {
    state_t state;

    uint8_t  din[3];
    uint8_t  cmd_in;

    uint8_t  reg_a;
    uint8_t  reg_b;
    uint8_t  reg_cmd;
    uint16_t reg_out;
    uint16_t alu_result;
    uint16_t mem_data_out;

    bool     flag_zero;
    bool     flag_error;
    bool     invalid_data;

    bool     aluin_reg_en;
    bool     datain_reg_en;
    bool     aluout_reg_en;
    bool     memory_read;
    bool     memory_write;
    bool     selmux2;

    bool     cpu_rdy;

    uint16_t memory[8];
} cpu_t;

uint16_t alu_executar(uint8_t opcode, uint8_t a, uint8_t b, bool invalid_data, bool *zero, bool *error);
void     decode_ciclo_fetch(cpu_t *cpu);
void     executar(cpu_t *cpu);
void     store(cpu_t *cpu);
uint8_t  seleciona_mux(uint8_t sel, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t feedback);

#endif