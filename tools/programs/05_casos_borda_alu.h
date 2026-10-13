// gerado automaticamente pelo assembler - nao editar

typedef struct
{
    unsigned char cmd;
    unsigned char din1;
    unsigned char din2;
    unsigned char din3;
    char fonte[256];
} InstrucaoPrograma;

#define QUANTIDADE_INSTRUCOES_PROGRAMA 9

static const InstrucaoPrograma PROGRAMA[] =
{
    { 0x0B, 100, 0, 0, "div din1, din2" },
    { 0x60, 10, 0, 0, "add dout_high, din1" },
    { 0x08, 5, 10, 0, "add din1, din2" },
    { 0x09, 5, 10, 0, "sub din1, din2" },
    { 0x09, 42, 42, 0, "sub din1, din2" },
    { 0x0A, 255, 255, 0, "mul din1, din2" },
    { 0x0A, 255, 0, 0, "mul din1, din2" },
    { 0x04, 255, 0, 0, "nop" },
    { 0x07, 255, 0, 0, "nop2" }
};
