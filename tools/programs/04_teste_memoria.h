// gerado automaticamente pelo assembler - nao editar

typedef struct
{
    unsigned char cmd;
    unsigned char din1;
    unsigned char din2;
    unsigned char din3;
    char fonte[256];
} InstrucaoPrograma;

#define QUANTIDADE_INSTRUCOES_PROGRAMA 25

static const InstrucaoPrograma PROGRAMA[] =
{
    { 0x08, 10, 1, 0, "add din1, din2" },
    { 0x46, 10, 1, 0, "store din3" },
    { 0x08, 20, 2, 0, "add din1, din2" },
    { 0x46, 20, 2, 1, "store din3" },
    { 0x08, 30, 3, 1, "add din1, din2" },
    { 0x46, 30, 3, 2, "store din3" },
    { 0x08, 40, 4, 2, "add din1, din2" },
    { 0x46, 40, 4, 3, "store din3" },
    { 0x08, 50, 5, 3, "add din1, din2" },
    { 0x46, 50, 5, 4, "store din3" },
    { 0x08, 60, 6, 4, "add din1, din2" },
    { 0x46, 60, 6, 5, "store din3" },
    { 0x08, 70, 7, 5, "add din1, din2" },
    { 0x46, 70, 7, 6, "store din3" },
    { 0x08, 80, 8, 6, "add din1, din2" },
    { 0x46, 80, 8, 7, "store din3" },
    { 0x45, 80, 8, 7, "load din3" },
    { 0x45, 80, 8, 6, "load din3" },
    { 0x45, 80, 8, 5, "load din3" },
    { 0x45, 80, 8, 4, "load din3" },
    { 0x45, 80, 8, 3, "load din3" },
    { 0x45, 80, 8, 2, "load din3" },
    { 0x45, 80, 8, 1, "load din3" },
    { 0x45, 80, 8, 0, "load din3" },
    { 0x04, 80, 8, 0, "nop" }
};
