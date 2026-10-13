#include "assembler.h"

static void extrair_nome_base(const char *caminho, char *base, size_t tamanho_max)
{
    strncpy(base, caminho, tamanho_max - 1);
    base[tamanho_max - 1] = '\0';

    char *ponto = strrchr(base, '.');
    char *barra1 = strrchr(base, '/');
    char *barra2 = strrchr(base, '\\');
    char *ultima_barra = (barra1 > barra2) ? barra1 : barra2;

    if (ponto != NULL && (ultima_barra == NULL || ponto > ultima_barra))
    {
        *ponto = '\0';
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "uso: %s <programa.asm> [-o <nome_base>]\n", argv[0]);
        return 1;
    }

    const char *caminho_entrada = NULL;
    const char *param_saida = NULL;

    for (int i = 1; i < argc; i = i + 1)
    {
        if ((strcmp(argv[i], "-o") == 0) && (i + 1 < argc))
        {
            param_saida = argv[i + 1];
            i = i + 1;
        }
        else if (argv[i][0] != '-')
        {
            caminho_entrada = argv[i];
        }
    }

    if (caminho_entrada == NULL)
    {
        fprintf(stderr, "erro: nenhum arquivo de entrada especificado\n");
        return 1;
    }

    char base_saida[256];
    if (param_saida != NULL)
    {
        extrair_nome_base(param_saida, base_saida, sizeof(base_saida));
    }
    else
    {
        extrair_nome_base(caminho_entrada, base_saida, sizeof(base_saida));
    }

    FILE *arquivo_entrada = fopen(caminho_entrada, "r");
    if (arquivo_entrada == NULL)
    {
        fprintf(stderr, "erro: nao consegui abrir '%s'\n", caminho_entrada);
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
            fclose(arquivo_entrada);
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

    char nome_mem[256];
    char nome_h[256];
    char nome_lst[256];
    char nome_stim[256];
    char nome_test[256];

    snprintf(nome_mem, sizeof(nome_mem), "%s.mem", base_saida);
    snprintf(nome_h, sizeof(nome_h), "%s.h", base_saida);
    snprintf(nome_lst, sizeof(nome_lst), "%s.lst", base_saida);
    snprintf(nome_stim, sizeof(nome_stim), "%s.stim", base_saida);
    snprintf(nome_test, sizeof(nome_test), "%s.test", base_saida);

    int resultado_mem = gerar_mem(programa, resultado_montagem, nome_mem);
    if (resultado_mem != 0)
    {
        return 1;
    }
    printf("gerado: %s com %d instrucoes\n", nome_mem, resultado_montagem);

    int resultado_header = gerar_header(programa, resultado_montagem, nome_h);
    if (resultado_header != 0)
    {
        return 1;
    }
    printf("gerado: %s com %d instrucoes\n", nome_h, resultado_montagem);

    int resultado_lst = gerar_lst(programa, resultado_montagem, nome_lst);
    if (resultado_lst != 0)
    {
        return 1;
    }
    printf("gerado: %s com %d instrucoes\n", nome_lst, resultado_montagem);

    int resultado_stim = gerar_stim(programa, resultado_montagem, nome_stim);
    if (resultado_stim != 0)
    {
        return 1;
    }
    printf("gerado: %s com %d instrucoes\n", nome_stim, resultado_montagem);

    int resultado_test = gerar_test(programa, resultado_montagem, nome_test);
    if (resultado_test != 0)
    {
        return 1;
    }
    printf("gerado: %s com %d instrucoes\n", nome_test, resultado_montagem);

    return 0;
}