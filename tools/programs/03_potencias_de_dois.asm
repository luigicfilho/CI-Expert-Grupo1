; 03_potencias_de_dois.asm
; Calculo progressivo de potencias de 2 (2^1 ate 2^8):
; 2^1 = 2   (0x0002)
; 2^2 = 4   (0x0004)
; 2^3 = 8   (0x0008)
; 2^4 = 16  (0x0010)
; 2^5 = 32  (0x0020)
; 2^6 = 64  (0x0040)
; 2^7 = 128 (0x0080)
; 2^8 = 256 (0x0100 - transbordo do byte baixo para o byte alto: high=1, low=0)

.echo "=== Teste Potencias de Dois ==="

set din1 = 2
set din2 = 1
mul din1, din2
.expect 0x0002, 0, 0

; 2 * 2 = 4
mul din1, dout_low
.expect 0x0004, 0, 0

; 4 * 2 = 8
mul din1, dout_low
.expect 0x0008, 0, 0

; 8 * 2 = 16 (0x0010)
mul din1, dout_low
.expect 0x0010, 0, 0

; 16 * 2 = 32 (0x0020)
mul din1, dout_low
.expect 0x0020, 0, 0

; 32 * 2 = 64 (0x0040)
mul din1, dout_low
.expect 0x0040, 0, 0

; 64 * 2 = 128 (0x0080)
mul din1, dout_low
.expect 0x0080, 0, 0

; 128 * 2 = 256 (0x0100)
mul din1, dout_low
.expect 0x0100, 0, 0

nop
.echo "=== Potencias de 2 Finalizado com Sucesso ==="
