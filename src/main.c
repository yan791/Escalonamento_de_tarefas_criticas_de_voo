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
    } 
    else if (!strcmp(argv[1], "edf")) {
        algoritmo = EDF;
    }
     else {
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

    if (sim.quantidade < 2) {
        fprintf(stderr,"Erro: o teste precisa de pelo menos duas tarefas.\n");
        liberar_dados(&sim);
        return 1;
    }

    for (i = 0; i < 10; i++) {
        int tarefa = i < 5 ? 0 : 1;

        if (!guardar_trecho(&hist, i, tarefa)) {
            fprintf(stderr, "Erro ao armazenar trecho.\n");
            free(hist.trechos);
            liberar_dados(&sim);
            return 1;
        }
    }

    if (hist.qtd_trechos != 2) {
        fprintf(stderr,"Erro: esperado 2 trechos, obtido %d.\n",hist.qtd_trechos);
        free(hist.trechos);
        liberar_dados(&sim);
        return 1;
    }

    printf("Teste de guardar_trecho: OK\n");

    sim.tarefas[0].ativa = 1;
    sim.tarefas[1].ativa = 1;

    sim.tarefas[0].restante = 2;
    sim.tarefas[1].restante = 3;

    sim.tarefas[0].deadline_absoluto = 10;
    sim.tarefas[1].deadline_absoluto = 7;

    
    if (escolher_tarefa(&sim, RATE) != 0) {
        fprintf(stderr, "Erro no teste do algoritmo RATE.\n");
        free(hist.trechos);
        liberar_dados(&sim);
        return 1;
    }

    printf("Teste de escolha por RATE: OK\n");

    if (escolher_tarefa(&sim, EDF) != 1) {
        fprintf(stderr, "Erro no teste do algoritmo EDF.\n");
        free(hist.trechos);
        liberar_dados(&sim);
        return 1;
    }

    printf("Teste de escolha por EDF: OK\n");
    checar_prazos(&sim, 7);

    if (sim.tarefas[1].ativa || sim.tarefas[1].restante != 0 || sim.tarefas[1].perdidas != 1) {
        fprintf(stderr,"Erro ao contabilizar o deadline perdido.\n");
        free(hist.trechos);
        liberar_dados(&sim);
        return 1;
    }

    if (!sim.tarefas[0].ativa) {
        fprintf(
            stderr,
            "Erro: tarefa com deadline futuro foi removida.\n"
        );
        free(hist.trechos);
        liberar_dados(&sim);
        return 1;
    }

    printf("Teste de deadline perdido: OK\n");
    criar_instancias(&sim, 0);

    for (i = 0; i < sim.quantidade; i++) {
        Tarefa *tarefa = &sim.tarefas[i];

        if (!tarefa->ativa ||
            tarefa->restante != tarefa->burst ||
            tarefa->chegada != 0 ||
            tarefa->deadline_absoluto != tarefa->deadline ||
            tarefa->liberadas != 1) {
            fprintf(stderr,"Erro ao criar instancia da tarefa %s.\n",tarefa->nome);
            free(hist.trechos);
            liberar_dados(&sim);
            return 1;
        }
    }

    printf("Teste de criacao das instancias: OK\n");

    printf("Algoritmo selecionado: %s\n",algoritmo == RATE ? "RATE" : "EDF");
    printf("Tarefas carregadas: %d\n", sim.quantidade);
    printf("Trechos armazenados: %d\n", hist.qtd_trechos);
    printf("Capacidade do historico: %d\n", hist.cap_trechos);

    free(hist.trechos);
    liberar_dados(&sim);

    return 0;
}