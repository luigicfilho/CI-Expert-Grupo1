#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_LINHA 256
#define MAXIMO_TOKENS 4

typedef enum
{
    LINHA_VAZIA,
    LINHA_SET,
    LINHA_EXPECT,
    LINHA_ECHO,
    LINHA_INSTRUCAO
} TipoDeLinha;

typedef struct
{
    int numero;
    TipoDeLinha tipo;
    char texto[TAMANHO_LINHA];
    char tokens[MAXIMO_TOKENS][32];
    int quantidade_tokens;
} LinhaAssembly;

typedef struct
{
    int  numero_linha;
    int  cmd;
    int  valor_din1;
    int  valor_din2;
    int  valor_din3;
    char texto_original[256];
} InstrucaoMontada;

typedef struct
{
    int valor_din1;
    int valor_din2;
    int valor_din3;
} EstadoFios;

void remover_excessos(char linha[]);
int separar_tokens(char linha[], char tokens[][32], int maximo_tokens);
TipoDeLinha classificar_linha(char tokens[][32]);
int montar_programa(LinhaAssembly linhas[], int quantidade_linhas, InstrucaoMontada instrucoes[], int limite_instrucoes);
int gerar_mem(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[]);
int gerar_header(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[]);
int gerar_lst(InstrucaoMontada programa[], int quantidade, const char nome_arquivo[]);