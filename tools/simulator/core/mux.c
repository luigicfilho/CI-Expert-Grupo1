#include <stdint.h>
#include "cpu_model.h"

uint8_t seleciona_mux(uint8_t sel, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t feedback) {
    switch (sel) {
        case 0:
            return d0;
        case 1:
            return d1;
        case 2:
            return d2;
        case 3:
            return feedback;
        default:
            return 0;
    }
}