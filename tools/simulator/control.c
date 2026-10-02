#include "cpu_model.h"

void decode_ciclo_fetch (cpu_t *cpu) {
    uint8_t muxA   = (cpu->cmd_in >> 5) & 0x03;
    uint8_t muxB   = (cpu->cmd_in >> 3) & 0x03;

    cpu->reg_cmd = cpu->cmd_in & 0x7F;

    cpu->reg_a = seleciona_mux(muxA, cpu->din[0], cpu->din[1], cpu->din[2], (uint8_t)(cpu->reg_out >> 8));
    cpu->reg_b = seleciona_mux(muxB, cpu->din[0], cpu->din[1], cpu->din[2], (uint8_t)(cpu->reg_out & 0xFF));

    if ((muxA == 3 || muxB == 3) && cpu->flag_error)
        cpu->invalid_data = true;
    else
        cpu->invalid_data = false;
}

void executar(cpu_t *cpu){
    cpu->alu_result = alu_executar(cpu->reg_cmd & 0x07, 
                                cpu->reg_a, 
                                cpu->reg_b, 
                                cpu->invalid_data,  
                                &cpu->flag_zero, 
                                &cpu->flag_error);
}

void store(cpu_t *cpu) {
    if(cpu->memory_write && !cpu->memory_read) {
        cpu->memory[cpu->reg_a & 0x07] = cpu->reg_out;
    }   
}

void cpu_step(cpu_t *cpu) {
    uint8_t tmp[3];
    switch (cpu->state) {
        case RST:
            tmp[0] = cpu->din[0];
            tmp[1] = cpu->din[1];
            tmp[2] = cpu->din[2];
            *cpu = (cpu_t){0};
            cpu->din[0] = tmp[0];
            cpu->din[1] = tmp[1];
            cpu->din[2] = tmp[2];
            cpu->state = FETCH_DECODE;
            break;
        case FETCH_DECODE:
            cpu->memory_read = false;
            cpu->memory_write = false;
            cpu->selmux2 = false;
            cpu->aluout_reg_en = false;

            cpu->cpu_rdy = true;
            cpu->aluin_reg_en = true;
            cpu->datain_reg_en = true;
            decode_ciclo_fetch (cpu);
            cpu->state = EXECUTE;
            break;
        case EXECUTE:
            cpu->cpu_rdy = false;
            if ((cpu->reg_cmd & 0x07) == OP_LOAD){
                cpu->memory_read = true;
                cpu->selmux2 = true;
                cpu->mem_data_out = cpu->memory[cpu->reg_a & 0x07];
            }
            executar(cpu);
            cpu->state = STORE;
            break;
        case STORE:
            cpu->aluout_reg_en = true;
            if ((cpu->reg_cmd & 0x07) == OP_STORE){
                cpu->memory_write = true;
                store(cpu);
            }
            
            if (cpu->selmux2) {
                cpu->reg_out = cpu->mem_data_out;
            } else {
                cpu->reg_out = cpu->alu_result;
            }
            
            cpu->state = FETCH_DECODE;
            break;
        default:
            cpu->state = RST;
            break;
    }
}