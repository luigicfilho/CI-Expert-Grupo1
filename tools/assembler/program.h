// gerado automaticamente pelo assembler - nao editar

typedef struct
{
    unsigned char cmd;
    unsigned char din1;
    unsigned char din2;
    unsigned char din3;
    char fonte[256];
} InstrucaoPrograma;

#define QUANTIDADE_INSTRUCOES_PROGRAMA 5

static const InstrucaoPrograma PROGRAMA[] =
{
    { 0x08, 10, 32, 0, "ADD  DIN1, DIN2" },
    { 0x6A, 10, 32, 0, "MUL  DOUT_HIGH, DIN2" },
    { 0x46, 10, 32, 3, "STORE DIN3" },
    { 0x45, 10, 32, 3, "LOAD  DIN3" },
    { 0x04, 10, 32, 3, "NOP" }
};
