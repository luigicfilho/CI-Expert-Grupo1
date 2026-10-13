// gerado automaticamente pelo assembler - nao editar

typedef struct
{
    unsigned char cmd;
    unsigned char din1;
    unsigned char din2;
    unsigned char din3;
    char fonte[256];
} InstrucaoPrograma;

#define QUANTIDADE_INSTRUCOES_PROGRAMA 6

static const InstrucaoPrograma PROGRAMA[] =
{
    { 0x0A, 1, 2, 0, "mul din1, din2" },
    { 0x1A, 3, 2, 0, "mul din1, dout_low" },
    { 0x1A, 4, 2, 0, "mul din1, dout_low" },
    { 0x1A, 5, 2, 0, "mul din1, dout_low" },
    { 0x1A, 6, 2, 0, "mul din1, dout_low" },
    { 0x04, 6, 2, 0, "nop" }
};
