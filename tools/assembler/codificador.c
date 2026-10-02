#include "assembler.h"
#define QUANTIDADE_INSTRUCOES 8

static const char operandos_muxA[4][10] = { "DIN1", "DIN2", "DIN3", "DOUT_HIGH" };
static const char operandos_muxB[4][10] = { "DIN1", "DIN2", "DIN3", "DOUT_LOW"  };
static const char nomes_instrucoes[QUANTIDADE_INSTRUCOES][10] = { "ADD", "SUB", "MUL", "DIV", "NOP", "LOAD", "STORE", "NOP2" };

static int buscar_operando(const char operando[], const char tabela[][10], int tamanho_tabela)
{
    for (int i = 0; i < tamanho_tabela; i = i + 1)
    {
        if (strcmp(operando, tabela[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

static int buscar_opcode(const char mnemonico[])
{
    for (int i = 0; i < QUANTIDADE_INSTRUCOES; i = i + 1)
    {
        if (strcmp(mnemonico, nomes_instrucoes[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

static int validar_operandos(char tokens[][32], int quantidade_tokens, int numero_instrucao, int *muxA, int *muxB)
{
    char mnemonico[32];
    strcpy(mnemonico, tokens[0]);
    int opcode = buscar_opcode(mnemonico);
    if (opcode <= 3)
    {
        if (quantidade_tokens != 3)
        {
            printf("erro linha %d: %s exige 2 operandos\n", numero_instrucao, mnemonico);
            return -1;
        }
        *muxA = buscar_operando(tokens[1], operandos_muxA, 4);
        *muxB = buscar_operando(tokens[2], operandos_muxB, 4);
        if (*muxA == -1)
        {
            printf("erro linha %d: operando '%s' invalido para o muxA\n", numero_instrucao, tokens[1]);
            return -1;
        }
        if (*muxB == -1)
        {
            printf("erro linha %d: operando '%s' invalido para o muxB\n", numero_instrucao, tokens[2]);
            return -1;
        }
        return 0;
    }
    if ((opcode == 5) || (opcode == 6))
    {
        if (quantidade_tokens != 2)
        {
            printf("erro linha %d: %s exige 1 operando\n", numero_instrucao, mnemonico);
            return -1;
        }
        *muxA = buscar_operando(tokens[1], operandos_muxA, 4);
        *muxB = 0;
        if (*muxA == -1)
        {
            printf("erro linha %d: operando '%s' invalido para o muxA\n", numero_instrucao, tokens[1]);
            return -1;
        }
        if (*muxA == 3)
        {
            printf("aviso linha %d: %s com DOUT_HIGH - o endereco depende do resultado anterior\n", numero_instrucao, mnemonico);
        }
        return 0;
    }
    if (quantidade_tokens != 1)
    {
        printf("erro linha %d: %s nao leva operando\n", numero_instrucao, mnemonico);
        return -1;
    }
    *muxA = 0;
    *muxB = 0;
    return 0;
}

int montar_programa(LinhaAssembly linhas[], int quantidade_linhas, InstrucaoMontada instrucoes[], int limite_instrucoes)
{
    EstadoFios fios;
    fios.valor_din1 = 0;
    fios.valor_din2 = 0;
    fios.valor_din3 = 0;

    int quantidade_montadas = 0;

    for (int i = 0; i < quantidade_linhas; i = i + 1)
    {
        LinhaAssembly linha_atual = linhas[i];

        if (linha_atual.tipo == LINHA_SET)
        {
            int novo_valor = atoi(linha_atual.tokens[3]);

            if (novo_valor < 0 || novo_valor > 255)
            {
                printf("erro linha %d: valor %d fora do alcance do barramento (0 a 255)\n", linha_atual.numero, novo_valor);
                return -1;
            }

            if (strcmp(linha_atual.tokens[1], "DIN1") == 0)
            {
                fios.valor_din1 = novo_valor;
            }

            if (strcmp(linha_atual.tokens[1], "DIN2") == 0)
            {
                fios.valor_din2 = novo_valor;
            }

            if (strcmp(linha_atual.tokens[1], "DIN3") == 0)
            {
                fios.valor_din3 = novo_valor;
            }
        }

        if (linha_atual.tipo == LINHA_INSTRUCAO)
        {
            if (quantidade_montadas >= limite_instrucoes)
            {
                printf("erro: programa passou de %d instrucoes\n", limite_instrucoes);
                return -1;
            }

            InstrucaoMontada nova;
            nova.numero_linha = linha_atual.numero;
            strcpy(nova.texto_original, linha_atual.texto);
            nova.valor_din1 = fios.valor_din1;
            nova.valor_din2 = fios.valor_din2;
            nova.valor_din3 = fios.valor_din3;

            int muxA = 0;
            int muxB = 0;
            int resultado_validacao = validar_operandos(linha_atual.tokens, linha_atual.quantidade_tokens, linha_atual.numero, &muxA, &muxB);

            if (resultado_validacao != 0)
            {
                return -1;
            }

            int opcode = buscar_opcode(linha_atual.tokens[0]);
            nova.cmd = (muxA << 5) | (muxB << 3) | opcode;

            instrucoes[quantidade_montadas] = nova;
            quantidade_montadas = quantidade_montadas + 1;
        }
    }

    return quantidade_montadas;
}