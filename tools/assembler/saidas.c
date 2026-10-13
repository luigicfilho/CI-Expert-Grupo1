#include "assembler.h"

int gerar_mem(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[])
{
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL)
    {
        printf("erro: nao consegui criar '%s'\n", nome_arquivo);
        return -1;
    }

    for (int i = 0; i < quantidade; i = i + 1)
    {
        fprintf(arquivo_saida, "%02X\n", programa[i].cmd);
    }

    fclose(arquivo_saida);
    return 0;
}

int gerar_header(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[])
{
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL)
    {
        printf("erro: nao consegui criar '%s'\n", nome_arquivo);
        return -1;
    }

    fprintf(arquivo_saida, "// gerado automaticamente pelo assembler - nao editar\n\n");
    fprintf(arquivo_saida, "typedef struct\n{\n");
    fprintf(arquivo_saida, "    unsigned char cmd;\n");
    fprintf(arquivo_saida, "    unsigned char din1;\n");
    fprintf(arquivo_saida, "    unsigned char din2;\n");
    fprintf(arquivo_saida, "    unsigned char din3;\n");
    fprintf(arquivo_saida, "    char fonte[%d];\n", TAMANHO_LINHA);
    fprintf(arquivo_saida, "} InstrucaoPrograma;\n\n");
    fprintf(arquivo_saida, "#define QUANTIDADE_INSTRUCOES_PROGRAMA %d\n\n", quantidade);
    fprintf(arquivo_saida, "static const InstrucaoPrograma PROGRAMA[] =\n{\n");

    for (int i = 0; i < quantidade; i = i + 1)
    {
        fprintf(arquivo_saida, "    { 0x%02X, %d, %d, %d, \"%s\" }",
                programa[i].cmd,
                programa[i].valor_din1,
                programa[i].valor_din2,
                programa[i].valor_din3,
                programa[i].texto_original);

        if (i < quantidade - 1)
        {
            fprintf(arquivo_saida, ",");
        }

        fprintf(arquivo_saida, "\n");
    }

    fprintf(arquivo_saida, "};\n");
    fclose(arquivo_saida);
    return 0;
}

int gerar_lst(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[])
{
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL)
    {
        printf("erro: nao consegui criar '%s'\n", nome_arquivo);
        return -1;
    }
    fprintf(arquivo_saida, "assembler listing - %d instrucoes\n\n", quantidade);
    fprintf(arquivo_saida, "#    cmd   binario    din1 din2 din3  fonte\n");
    for (int i = 0; i < quantidade; i = i + 1)
    {
        int muxA = (programa[i].cmd >> 5) & 0x03;
        int muxB = (programa[i].cmd >> 3) & 0x03;
        int opcode = programa[i].cmd & 0x07;
        if (programa[i].tem_expect)
        {
            fprintf(arquivo_saida, "%-3d  0x%02X  %d%d %d%d %d%d%d %4d %4d %4d   %-24s  [expect out=0x%04X err=%d zero=%d]\n",
                    i,
                    programa[i].cmd,
                    (muxA >> 1) & 1, muxA & 1,
                    (muxB >> 1) & 1, muxB & 1,
                    (opcode >> 2) & 1, (opcode >> 1) & 1, opcode & 1,
                    programa[i].valor_din1,
                    programa[i].valor_din2,
                    programa[i].valor_din3,
                    programa[i].texto_original,
                    programa[i].expect_dout & 0xFFFF,
                    programa[i].expect_error ? 1 : 0,
                    programa[i].expect_zero ? 1 : 0);
        }
        else
        {
            fprintf(arquivo_saida, "%-3d  0x%02X  %d%d %d%d %d%d%d %4d %4d %4d   %s\n",
                    i,
                    programa[i].cmd,
                    (muxA >> 1) & 1, muxA & 1,
                    (muxB >> 1) & 1, muxB & 1,
                    (opcode >> 2) & 1, (opcode >> 1) & 1, opcode & 1,
                    programa[i].valor_din1,
                    programa[i].valor_din2,
                    programa[i].valor_din3,
                    programa[i].texto_original);
        }
    }
    fclose(arquivo_saida);
    return 0;
}

int gerar_stim(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[])
{
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL)
    {
        printf("erro: nao consegui criar '%s'\n", nome_arquivo);
        return -1;
    }

    for (int i = 0; i < quantidade; i = i + 1)
    {
        fprintf(arquivo_saida, "%d %d %d %d\n",
                programa[i].valor_din1,
                programa[i].valor_din2,
                programa[i].valor_din3,
                programa[i].cmd);
    }

    fclose(arquivo_saida);
    return 0;
}

int gerar_test(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[])
{
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL)
    {
        printf("erro: nao consegui criar '%s'\n", nome_arquivo);
        return -1;
    }

    fprintf(arquivo_saida, "; gerado automaticamente pelo assembler - %d instrucoes\n", quantidade);
    fprintf(arquivo_saida, "; formato: din1 din2 din3 cmd [expected_dout expected_error expected_zero]\n\n");

    for (int i = 0; i < quantidade; i = i + 1)
    {
        if (programa[i].tem_expect)
        {
            fprintf(arquivo_saida, "%d %d %d %d 0x%04X %d %d\n",
                    programa[i].valor_din1,
                    programa[i].valor_din2,
                    programa[i].valor_din3,
                    programa[i].cmd,
                    programa[i].expect_dout & 0xFFFF,
                    programa[i].expect_error ? 1 : 0,
                    programa[i].expect_zero ? 1 : 0);
        }
        else
        {
            fprintf(arquivo_saida, "%d %d %d %d\n",
                    programa[i].valor_din1,
                    programa[i].valor_din2,
                    programa[i].valor_din3,
                    programa[i].cmd);
        }
    }

    fclose(arquivo_saida);
    return 0;
}