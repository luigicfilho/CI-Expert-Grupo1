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