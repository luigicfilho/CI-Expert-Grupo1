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
    { 0x0A, 2, 1, 0, "mul din1, din2" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x1A, 2, 1, 0, "mul din1, dout_low" },
    { 0x04, 2, 1, 0, "nop" }
};
