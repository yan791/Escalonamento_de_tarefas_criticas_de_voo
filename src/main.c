#include <stdio.h>
#include <string.h>
#include "scheduler.h"

void mostrar_uso(const char *programa) {
    fprintf(stderr, "Uso: %s <rate|edf> <arquivo_de_entrada>\n", programa);
}

int main(int argc, char **argv) {
    Algoritmo algoritmo;
    const char *saida;
    Simulacao sim = {0};

    if (argc != 3) {
        mostrar_uso(argv[0]);
        return 1;
    }

    if (!strcmp(argv[1], "rate")) {
        algoritmo = RATE;
    } 
    else if (!strcmp(argv[1], "edf")) {
        algoritmo = EDF;
    } 
    else {
        fprintf(stderr,"Erro: algoritmo invalido '%s'. Use rate ou edf.\n",argv[1]);
        return 1;
    }

    saida = algoritmo == RATE
        ? "rate_yrn.out"
        : "edf_yrn.out";

    if (!ler_entrada(argv[2], &sim)) {
        return 1;
    }

    if (!executar(&sim, algoritmo, saida)) {
        liberar_dados(&sim);
        return 1;
    }

    liberar_dados(&sim);
    return 0;
}