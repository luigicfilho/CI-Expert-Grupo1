#include <stdint.h>
#include <stdbool.h>
#include "cpu_model.h"

uint16_t alu_executar(uint8_t opcode, uint8_t a, uint8_t b, bool invalid_data, bool *zero, bool *error) {
    uint16_t resultado;
    *zero = false;
    *error = false;

    if (invalid_data) {
        *error = true;
        return 0xFFFF;
    }

    switch (opcode) {
        case OP_ADD:
            resultado = a + b;
            break;
        case OP_SUB:
            resultado = a - b;
            break;
        case OP_MUL:
            resultado = a * b;
            break;
        case OP_DIV:
            if (b == 0){
                *error = true;
                resultado = 0xFFFF;
                break;
            }
            resultado = a / b;
            break;
        default:
            resultado = 0;
            break;
    }

    *zero  = (resultado == 0);

    return resultado;
}

