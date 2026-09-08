#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler.h"

void mostrar_uso(const char *programa) {
    fprintf(stderr, "Uso: %s <rate|edf> <arquivo_de_entrada>\n", programa);
}

int main(int argc, char **argv) {
    Algoritmo algoritmo;
    Simulacao sim = {0};
    Historico hist = {0};
    int i;

    if (argc != 3) {
        mostrar_uso(argv[0]);
        return 1;
    }

    if (!strcmp(argv[1], "rate")) {
        algoritmo = RATE;
    } else if (!strcmp(argv[1], "edf")) {
        algoritmo = EDF;
    } else {
        fprintf(
            stderr,
            "Erro: algoritmo invalido '%s'. Use rate ou edf.\n",
            argv[1]
        );
        return 1;
    }

    if (!ler_entrada(argv[2], &sim)) {
        return 1;
    }

    for (i = 0; i < 10; i++) {
        int tarefa = i % sim.quantidade;

        if (!guardar_trecho(&hist, i, tarefa)) {
            fprintf(stderr, "Erro ao armazenar trecho.\n");
            free(hist.trechos);
            liberar_dados(&sim);
            return 1;
        }
    }

    printf(
        "Algoritmo selecionado: %s\n",
        algoritmo == RATE ? "RATE" : "EDF"
    );
    printf("Tarefas carregadas: %d\n", sim.quantidade);
    printf("Trechos armazenados: %d\n", hist.qtd_trechos);
    printf("Capacidade do historico: %d\n", hist.cap_trechos);

    free(hist.trechos);
    liberar_dados(&sim);

    return 0;
}