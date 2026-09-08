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

void liberar_dados(Simulacao *sim) {
    free(sim->tarefas);
    sim->tarefas = NULL;
    sim->quantidade = 0;
    sim->tempo_total = 0;
}