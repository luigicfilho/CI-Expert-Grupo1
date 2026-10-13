; 02_fatorial.asm
; Calculo iterativo de fatorial encadeado utilizando realimentacao (feedback):
; 1! = 1
; 2! = 1 * 2 = 2
; 3! = 2 * 3 = 6
; 4! = 6 * 4 = 24  (0x0018)
; 5! = 24 * 5 = 120 (0x0078)
; 6! = 120 * 6 = 720 (0x02D0 - testa resultado de 16 bits: high=0x02, low=0xD0)

.echo "=== Teste Fatorial Encadeado com Feedback ==="

; 1 * 2 = 2
set din1 = 1
set din2 = 2
mul din1, din2
.expect 0x0002, 0, 0

; 2! * 3 = 6 (usando DOUT_LOW como primeiro operando)
set din1 = 3
mul din1, dout_low
.expect 0x0006, 0, 0

; 3! * 4 = 24
set din1 = 4
mul din1, dout_low
.expect 0x0018, 0, 0

; 4! * 5 = 120 (0x0078)
set din1 = 5
mul din1, dout_low
.expect 0x0078, 0, 0

; 5! * 6 = 720 (0x02D0) - gera resultado maior que 8 bits (255)
set din1 = 6
mul din1, dout_low
.expect 0x02D0, 0, 0

nop
.echo "=== Fatorial Finalizado com Sucesso ==="
