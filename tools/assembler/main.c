#include "assembler.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "uso: %s programa.asm\n", argv[0]);
        return 1;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (arquivo_entrada == NULL)
    {
        fprintf(stderr, "erro: nao consegui abrir '%s'\n", argv[1]);
        return 1;
    }

    LinhaAssembly linhas_do_programa[1024];
    int quantidade_linhas = 0;

    char linha_bruta[TAMANHO_LINHA];
    int contador_linhas = 0;
    while (fgets(linha_bruta, sizeof(linha_bruta), arquivo_entrada) != NULL)
    {
        contador_linhas = contador_linhas + 1;

        remover_excessos(linha_bruta);

        if (linha_bruta[0] == '\0')
        {
            continue;
        }

        if (quantidade_linhas >= 1024)
        {
            fprintf(stderr, "erro: programa tem linhas demais\n");
            return 1;
        }

        LinhaAssembly linha_atual;
        linha_atual.numero = contador_linhas;
        strcpy(linha_atual.texto, linha_bruta);
        linha_atual.quantidade_tokens = separar_tokens(linha_bruta, linha_atual.tokens, MAXIMO_TOKENS);
        linha_atual.tipo = classificar_linha(linha_atual.tokens);

        linhas_do_programa[quantidade_linhas] = linha_atual;
        quantidade_linhas = quantidade_linhas + 1;
    }

    fclose(arquivo_entrada);

    InstrucaoMontada programa[512];
    int resultado_montagem = montar_programa(linhas_do_programa, quantidade_linhas, programa, 512);

    if (resultado_montagem < 0)
    {
        fprintf(stderr, "erro: montagem falhou\n");
        return 1;
    }

    for (int i = 0; i < resultado_montagem; i = i + 1)
    {
        printf("instr %d: cmd=0x%02X  din=(%d, %d, %d)  %s\n",
               i, programa[i].cmd,
               programa[i].valor_din1, programa[i].valor_din2, programa[i].valor_din3,
               programa[i].texto_original);
    }

    int resultado_mem = gerar_mem(programa, resultado_montagem, "program.mem");
    if (resultado_mem != 0)
    {
        return 1;
    }

    printf("gerado: program.mem com %d instrucoes\n", resultado_montagem);

    int resultado_header = gerar_header(programa, resultado_montagem, "program.h");
    if (resultado_header != 0)
    {
        return 1;
    }

    printf("gerado: program.h com %d instrucoes\n", resultado_montagem);

    int resultado_lst = gerar_lst(programa, resultado_montagem, "program.lst");
    if (resultado_lst != 0)
    {
        return 1;
    }

    printf("gerado: program.lst com %d instrucoes\n", resultado_montagem);

    return 0;
}