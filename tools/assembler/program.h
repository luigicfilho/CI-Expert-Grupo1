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
    { 0x08, 10, 32, 0, "add  din1, din2" },
    { 0x6A, 10, 32, 0, "mul  dout_high, din2" },
    { 0x46, 10, 32, 3, "store din3" },
    { 0x45, 10, 32, 3, "load  din3" },
    { 0x04, 10, 32, 3, "nop" },
    { 0x07, 10, 32, 3, "nop2" }
};
