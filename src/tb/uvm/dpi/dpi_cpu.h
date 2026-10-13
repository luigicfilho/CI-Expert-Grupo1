#ifndef DPI_CPU_H
#define DPI_CPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     dpi_cpu_reset(void);

void     dpi_cpu_predict_instruction(
             uint8_t   cmd,
             uint8_t   din1,
             uint8_t   din2,
             uint8_t   din3,
             uint16_t *out_dout,
             uint8_t  *out_zero,
             uint8_t  *out_error
         );

void     dpi_cpu_get_registers(
             uint8_t  *reg_a,
             uint8_t  *reg_b,
             uint8_t  *reg_cmd,
             uint16_t *reg_out
         );

uint16_t dpi_cpu_read_memory(uint8_t address);

#ifdef __cplusplus
}
#endif

#endif
