; 01_fibonacci.asm
; Calculo dos primeiros termos da sequencia de Fibonacci:
; F(1)=1, F(2)=1, F(3)=2, F(4)=3, F(5)=5, F(6)=8, F(7)=13, F(8)=21
; Os termos sao calculados e salvos na memoria interna (posicoes 1 a 7).
; Em seguida, a memoria e lida de volta para validar a persistencia.

.echo "=== Iniciando Teste: Sequencia de Fibonacci ==="

; F(1) = 1
set din1 = 0
set din2 = 1
add din1, din2
.expect 0x0001, 0, 0
set din3 = 1
store din3
.expect 0x0000, 0, 1

; F(2) + F(1) = 1 + 1 = 2 -> F(3)
set din1 = 1
set din2 = 1
add din1, din2
.expect 0x0002, 0, 0
set din3 = 2
store din3
.expect 0x0000, 0, 1

; F(3) + F(2) = 2 + 1 = 3 -> F(4)
set din1 = 2
set din2 = 1
add din1, din2
.expect 0x0003, 0, 0
set din3 = 3
store din3
.expect 0x0000, 0, 1

; F(4) + F(3) = 3 + 2 = 5 -> F(5)
set din1 = 3
set din2 = 2
add din1, din2
.expect 0x0005, 0, 0
set din3 = 4
store din3
.expect 0x0000, 0, 1

; F(5) + F(4) = 5 + 3 = 8 -> F(6)
set din1 = 5
set din2 = 3
add din1, din2
.expect 0x0008, 0, 0
set din3 = 5
store din3
.expect 0x0000, 0, 1

; F(6) + F(5) = 8 + 5 = 13 -> F(7)
set din1 = 8
set din2 = 5
add din1, din2
.expect 0x000D, 0, 0
set din3 = 6
store din3
.expect 0x0000, 0, 1

; F(7) + F(6) = 13 + 8 = 21 -> F(8)
set din1 = 13
set din2 = 8
add din1, din2
.expect 0x0015, 0, 0
set din3 = 7
store din3
.expect 0x0000, 0, 1

.echo "=== Leitura e Verificacao da Memoria ==="
set din3 = 1
load din3
.expect 0x0001, 0, 0

set din3 = 2
load din3
.expect 0x0002, 0, 0

set din3 = 3
load din3
.expect 0x0003, 0, 0

set din3 = 4
load din3
.expect 0x0005, 0, 0

set din3 = 5
load din3
.expect 0x0008, 0, 0

set din3 = 6
load din3
.expect 0x000D, 0, 0

set din3 = 7
load din3
.expect 0x0015, 0, 0

nop
.echo "=== Fibonacci Finalizado com Sucesso ==="
