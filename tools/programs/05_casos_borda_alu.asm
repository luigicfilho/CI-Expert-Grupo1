; 05_casos_borda_alu.asm
; Teste rigoroso de casos de borda e excecoes da ALU:
; 1. Divisao por zero (error=1, resultado=0xFFFF)
; 2. Tentativa de reuso de resultado com erro anterior (invalid_data -> error=1, resultado=0xFFFF)
; 3. Recuperacao de operacao com novas entradas limpas
; 4. Subtracao com underflow (resultado negativo de 16 bits em complemento de 2)
; 5. Subtracao resultando em zero (flag zero=1)
; 6. Multiplicacao maxima (255 * 255 = 65025 = 0xFE01)
; 7. Multiplicacao por zero (flag zero=1)
; 8. NOP e NOP2

.echo "=== 1. Teste de Divisao por Zero ==="
set din1 = 100
set din2 = 0
div din1, din2
.expect 0xFFFF, 1, 0

.echo "=== 2. Reuso de dado sob condicao de erro (invalid_data) ==="
; Como a instrucao anterior gerou erro, usar DOUT_HIGH dispara invalid_data na ALU
set din1 = 10
add dout_high, din1
.expect 0xFFFF, 1, 0

.echo "=== 3. Recuperacao com novas entradas sem realimentacao ==="
; Novas entradas DIN1 e DIN2 sem usar realimentacao limpam o erro
set din1 = 5
set din2 = 10
add din1, din2
.expect 0x000F, 0, 0

.echo "=== 4. Subtracao com Underflow (5 - 10 = -5 = 0xFFFB) ==="
set din1 = 5
set din2 = 10
sub din1, din2
.expect 0xFFFB, 0, 0

.echo "=== 5. Subtracao com Resultado Zero (42 - 42 = 0) ==="
set din1 = 42
set din2 = 42
sub din1, din2
.expect 0x0000, 0, 1

.echo "=== 6. Multiplicacao Maxima de 8 bits (255 * 255 = 0xFE01) ==="
set din1 = 255
set din2 = 255
mul din1, din2
.expect 0xFE01, 0, 0

.echo "=== 7. Multiplicacao por Zero (255 * 0 = 0) ==="
set din1 = 255
set din2 = 0
mul din1, din2
.expect 0x0000, 0, 1

.echo "=== 8. Instrucoes NOP e NOP2 ==="
nop
.expect 0x0000, 0, 1

nop2
.expect 0x0000, 0, 1

.echo "=== Todos os Casos de Borda da ALU Validados com Sucesso ==="
