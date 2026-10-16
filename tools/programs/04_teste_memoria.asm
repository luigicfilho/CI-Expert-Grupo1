; 04_teste_memoria.asm
; Teste exaustivo de persistencia nas 8 posicoes da memoria interna (0 a 7):
; 1. Grava valores distintos em todas as 8 posicoes.
; 2. Le as posicoes em ordem inversa garantindo que nao houve sobreposicao ou colisao.

.echo "=== Gravando nas 8 posicoes de memoria ==="

; Mem[0] = 11 (0x000B)
set din1 = 10
set din2 = 1
add din1, din2
.expect 0x000B, 0, 0
set din3 = 0
store din3
.expect 0x0000, 0, 1

; Mem[1] = 22 (0x0016)
set din1 = 20
set din2 = 2
add din1, din2
.expect 0x0016, 0, 0
set din3 = 1
store din3
.expect 0x0000, 0, 1

; Mem[2] = 33 (0x0021)
set din1 = 30
set din2 = 3
add din1, din2
.expect 0x0021, 0, 0
set din3 = 2
store din3
.expect 0x0000, 0, 1

; Mem[3] = 44 (0x002C)
set din1 = 40
set din2 = 4
add din1, din2
.expect 0x002C, 0, 0
set din3 = 3
store din3
.expect 0x0000, 0, 1

; Mem[4] = 55 (0x0037)
set din1 = 50
set din2 = 5
add din1, din2
.expect 0x0037, 0, 0
set din3 = 4
store din3
.expect 0x0000, 0, 1

; Mem[5] = 66 (0x0042)
set din1 = 60
set din2 = 6
add din1, din2
.expect 0x0042, 0, 0
set din3 = 5
store din3
.expect 0x0000, 0, 1

; Mem[6] = 77 (0x004D)
set din1 = 70
set din2 = 7
add din1, din2
.expect 0x004D, 0, 0
set din3 = 6
store din3
.expect 0x0000, 0, 1

; Mem[7] = 88 (0x0058)
set din1 = 80
set din2 = 8
add din1, din2
.expect 0x0058, 0, 0
set din3 = 7
store din3
.expect 0x0000, 0, 1

.echo "=== Lendo e verificando em ordem reversa (7 ate 0) ==="

set din3 = 7
load din3
.expect 0x0058, 0, 0

set din3 = 6
load din3
.expect 0x004D, 0, 0

set din3 = 5
load din3
.expect 0x0042, 0, 0

set din3 = 4
load din3
.expect 0x0037, 0, 0

set din3 = 3
load din3
.expect 0x002C, 0, 0

set din3 = 2
load din3
.expect 0x0021, 0, 0

set din3 = 1
load din3
.expect 0x0016, 0, 0

set din3 = 0
load din3
.expect 0x000B, 0, 0

nop
.echo "=== Teste de Memoria Finalizado com Sucesso ==="
