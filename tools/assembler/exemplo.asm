; exemplo.asm - teste completo do assembler

set din1 = 0x0A
set din2 = 32
.echo "soma simples: 10 + 32 = 42"
add  din1, din2
.expect 0x002A, 0, 0

.echo "encadeando com feedback: DOUT_HIGH (0) * 32 = 0"
mul  dout_high, din2
.expect 0x0000, 0, 1

set din3 = 3
store din3
.expect 0x0000, 0, 1

load  din3
.expect 0x0000, 0, 1

nop
.expect 0x0000, 0, 1

nop2
.expect 0x0000, 0, 1
