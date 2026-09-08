#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#define LINE_SIZE 256

int linha_vazia(const char *linha) {
    int i;

    for (i = 0; linha[i]; i++) {
        if (!isspace((unsigned char) linha[i])) {
            return 0;
        }
    }

    return 1;
}

int ler_tempo(const char *linha, int *tempo) {
    char sobra;

    return sscanf(linha, " %d %c", tempo, &sobra) == 1;
}

int ler_entrada(const char *nome_arquivo, Simulacao *sim) {
    FILE *arq = fopen(nome_arquivo, "r");
    char linha[LINE_SIZE];
    int numero_linha = 1;
    int capacidade = 0;

    if (!arq) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'.\n", nome_arquivo);
        return 0;
    }

    if (!fgets(linha, sizeof(linha), arq) || !ler_tempo(linha, &sim->tempo_total) || sim->tempo_total <= 0) {
        fprintf(stderr, "Erro: a primeira linha deve ter um inteiro positivo.\n");
        fclose(arq);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arq)) {
        Tarefa atual;
        char sobra;
        int campos;

        numero_linha++;

        if (linha_vazia(linha)) {
            continue;
        }

        campos = sscanf(
            linha,
            " %31s %d %d %d %c",
            atual.nome,
            &atual.periodo,
            &atual.deadline,
            &atual.burst,
            &sobra
        );

        if (campos != 4) {
            fprintf(stderr, "Erro: formato invalido na linha %d.\n", numero_linha);
            fclose(arq);
            return 0;
        }

        if (atual.periodo <= 0 || atual.deadline <= 0 || atual.burst <= 0) {
            fprintf(stderr, "Erro: valor nao positivo na linha %d.\n", numero_linha);
            fclose(arq);
            liberar_dados(sim);
            return 0;
        }

        if (sim->quantidade == capacidade) {
            capacidade = capacidade ? capacidade * 2 : 4;

            sim->tarefas = realloc(sim->tarefas,(size_t) capacidade * sizeof(Tarefa));
        }

        sim->tarefas[sim->quantidade] = atual;
        sim->quantidade++;
    }

    fclose(arq);
    return 1;
}

void liberar_dados(Simulacao *sim) {
    free(sim->tarefas);
    sim->tarefas = NULL;
    sim->quantidade = 0;
    sim->tempo_total = 0;
}