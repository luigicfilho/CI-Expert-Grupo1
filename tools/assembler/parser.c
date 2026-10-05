#include "assembler.h"

void remover_excessos(char linha[]) 
{
    int tamanho = (int)strlen(linha);

    for (int i = 0; i < tamanho; i = i + 1)
    {
        if (linha[i] == ';')
        {
            linha[i] = '\0';
            tamanho = i;
            break;
        }
    }

    int inicio = 0;
    while ((linha[inicio] == ' ') || (linha[inicio] == '\t') || (linha[inicio] == '\n') || (linha[inicio] == '\r'))
    {
        inicio = inicio + 1;
    }

    int fim = (int)strlen(linha) - 1;
    while ((fim >= inicio) && ((linha[fim] == ' ') || (linha[fim] == '\t') || (linha[fim] == '\n') || (linha[fim] == '\r')))
    {
        fim = fim - 1;
    }

    int escrita = 0;
    for (int leitura = inicio; leitura <= fim; leitura = leitura + 1)
    {
        linha[escrita] = linha[leitura];
        escrita = escrita + 1;
    }
    linha[escrita] = '\0';
}

int separar_tokens(char linha[], char tokens[][32], int maximo_tokens)
{
    int quantidade_tokens = 0;
    int posicao_atual = 0;
    int tamanho_linha = (int)strlen(linha);

    while (posicao_atual < tamanho_linha)
    {
        while ((posicao_atual < tamanho_linha) &&
               ((linha[posicao_atual] == ' ') ||
                (linha[posicao_atual] == '\t') ||
                (linha[posicao_atual] == ',')))
        {
            posicao_atual = posicao_atual + 1;
        }

        if (posicao_atual >= tamanho_linha)
        {
            break;
        }

        int posicao_token = 0;
        while ((posicao_atual < tamanho_linha) &&
               (linha[posicao_atual] != ' ') &&
               (linha[posicao_atual] != '\t') &&
               (linha[posicao_atual] != ','))
        {
            if (posicao_token < 31)
            {
                tokens[quantidade_tokens][posicao_token] = linha[posicao_atual];
                posicao_token = posicao_token + 1;
            }
            posicao_atual = posicao_atual + 1;
        }

        tokens[quantidade_tokens][posicao_token] = '\0';
        quantidade_tokens = quantidade_tokens + 1;

        if (quantidade_tokens >= maximo_tokens)
        {
            break;
        }
    }

    return quantidade_tokens;
}

TipoDeLinha classificar_linha(char tokens[][32])
{
    char mnemonico[32];
    strcpy(mnemonico, tokens[0]);

    TipoDeLinha tipo = LINHA_VAZIA;

    if (strcmp(mnemonico, "SET") == 0)
    {
        tipo = LINHA_SET;
    }

    if (strcmp(mnemonico, ".expect") == 0)
    {
        tipo = LINHA_EXPECT;
    }

    if (strcmp(mnemonico, ".echo") == 0)
    {
        tipo = LINHA_ECHO;
    }

    if ((strcmp(mnemonico, "ADD") == 0) ||
        (strcmp(mnemonico, "SUB") == 0) ||
        (strcmp(mnemonico, "MUL") == 0) ||
        (strcmp(mnemonico, "DIV") == 0) ||
        (strcmp(mnemonico, "NOP") == 0) ||
        (strcmp(mnemonico, "LOAD") == 0) ||
        (strcmp(mnemonico, "STORE") == 0))
    {
        tipo = LINHA_INSTRUCAO;
    }

    return tipo;
}
