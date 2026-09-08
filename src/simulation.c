#include <stdlib.h>
#include "scheduler.h"

int guardar_trecho(Historico *hist, int tempo, int tarefa) {
    Trecho *novo;

    if (hist->qtd_trechos) {
        Trecho *ultimo = &hist->trechos[hist->qtd_trechos - 1];

        if (ultimo->tarefa == tarefa && ultimo->fim == tempo) {
            ultimo->fim++;
            return 1;
        }
    }

    if (hist->qtd_trechos == hist->cap_trechos) {
        int nova_capacidade;

        nova_capacidade = hist->cap_trechos
            ? hist->cap_trechos * 2
            : 8;

        novo = realloc(hist->trechos,(size_t) nova_capacidade * sizeof(Trecho));

        if (!novo) {
            return 0;
        }

        hist->trechos = novo;
        hist->cap_trechos = nova_capacidade;
    }

    hist->trechos[hist->qtd_trechos].inicio = tempo;
    hist->trechos[hist->qtd_trechos].fim = tempo + 1;
    hist->trechos[hist->qtd_trechos].tarefa = tarefa;
    hist->trechos[hist->qtd_trechos].motivo = '?';
    hist->qtd_trechos++;

    return 1;
}
