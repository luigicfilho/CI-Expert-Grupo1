#include "dpi_cpu.h"

#if defined(__has_include)
    #if __has_include("cpu_model.h")
        #include "cpu_model.h"
    #else
        #include "../../../../tools/simulator/core/cpu_model.h"
    #endif
#else
    #include "cpu_model.h"
#endif

static cpu_t g_dpi_cpu;

void dpi_cpu_reset(void) {
    g_dpi_cpu = (cpu_t){0};
    cpu_step(&g_dpi_cpu);
}

void dpi_cpu_predict_instruction(
    uint8_t   cmd,
    uint8_t   din1,
    uint8_t   din2,
    uint8_t   din3,
    uint16_t *out_dout,
    uint8_t  *out_zero,
    uint8_t  *out_error
) {
    g_dpi_cpu.din[0] = din1;
    g_dpi_cpu.din[1] = din2;
    g_dpi_cpu.din[2] = din3;
    g_dpi_cpu.cmd_in = cmd;

    for (int i = 0; i < 3; i++) {
        cpu_step(&g_dpi_cpu);
    }

    if (out_dout) {
        *out_dout = g_dpi_cpu.reg_out;
    }
    if (out_zero) {
        *out_zero = g_dpi_cpu.flag_zero ? 1 : 0;
    }
    if (out_error) {
        *out_error = g_dpi_cpu.flag_error ? 1 : 0;
    }
}

void dpi_cpu_get_registers(
    uint8_t  *reg_a,
    uint8_t  *reg_b,
    uint8_t  *reg_cmd,
    uint16_t *reg_out
) {
    if (reg_a) {
        *reg_a = g_dpi_cpu.reg_a;
    }
    if (reg_b) {
        *reg_b = g_dpi_cpu.reg_b;
    }
    if (reg_cmd) {
        *reg_cmd = g_dpi_cpu.reg_cmd;
    }
    if (reg_out) {
        *reg_out = g_dpi_cpu.reg_out;
    }
}

uint16_t dpi_cpu_read_memory(uint8_t address) {
    return g_dpi_cpu.memory[address & 0x07];
}
