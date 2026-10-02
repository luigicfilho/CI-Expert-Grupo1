; exemplo.asm - teste do assembler

SET DIN1 = 10
SET DIN2 = 32
.echo "soma simples"
ADD  DIN1, DIN2
.echo "encadeando com feedback"
MUL  DOUT_HIGH, DIN2
SET DIN3 = 3
STORE DIN3
LOAD  DIN3
NOP
