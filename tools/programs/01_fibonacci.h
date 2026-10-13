// gerado automaticamente pelo assembler - nao editar

typedef struct
{
    unsigned char cmd;
    unsigned char din1;
    unsigned char din2;
    unsigned char din3;
    char fonte[256];
} InstrucaoPrograma;

#define QUANTIDADE_INSTRUCOES_PROGRAMA 22

static const InstrucaoPrograma PROGRAMA[] =
{
    { 0x08, 0, 1, 0, "add din1, din2" },
    { 0x46, 0, 1, 1, "store din3" },
    { 0x08, 1, 1, 1, "add din1, din2" },
    { 0x46, 1, 1, 2, "store din3" },
    { 0x08, 2, 1, 2, "add din1, din2" },
    { 0x46, 2, 1, 3, "store din3" },
    { 0x08, 3, 2, 3, "add din1, din2" },
    { 0x46, 3, 2, 4, "store din3" },
    { 0x08, 5, 3, 4, "add din1, din2" },
    { 0x46, 5, 3, 5, "store din3" },
    { 0x08, 8, 5, 5, "add din1, din2" },
    { 0x46, 8, 5, 6, "store din3" },
    { 0x08, 13, 8, 6, "add din1, din2" },
    { 0x46, 13, 8, 7, "store din3" },
    { 0x45, 13, 8, 1, "load din3" },
    { 0x45, 13, 8, 2, "load din3" },
    { 0x45, 13, 8, 3, "load din3" },
    { 0x45, 13, 8, 4, "load din3" },
    { 0x45, 13, 8, 5, "load din3" },
    { 0x45, 13, 8, 6, "load din3" },
    { 0x45, 13, 8, 7, "load din3" },
    { 0x04, 13, 8, 7, "nop" }
};
