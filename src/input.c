#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#define LINE_SIZE 256

void liberar_dados(Simulacao *sim) {
    free(sim->tarefas);
    sim->tarefas = NULL;
    sim->quantidade = 0;
    sim->tempo_total = 0;
}