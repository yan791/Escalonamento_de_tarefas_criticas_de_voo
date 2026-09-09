#include <stdio.h>
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

int escolher_tarefa(const Simulacao *sim, Algoritmo algoritmo) {
    int escolhida = -1;
    int i;

    for (i = 0; i < sim->quantidade; i++) {
        const Tarefa *candidata = &sim->tarefas[i];
        const Tarefa *atual;

        if (!candidata->ativa) {
            continue;
        }

        if (escolhida == -1) {
            escolhida = i;
            continue;
        }

        atual = &sim->tarefas[escolhida];

        if (algoritmo == RATE &&
            candidata->periodo < atual->periodo) {
            escolhida = i;
        } 
        else if (algoritmo == EDF && candidata->deadline_absoluto <   atual->deadline_absoluto) {
            escolhida = i;
        }
    }

    return escolhida;
}

void checar_prazos(Simulacao *sim, int tempo) {
    int i;

    for (i = 0; i < sim->quantidade; i++) {
        Tarefa *tarefa = &sim->tarefas[i];

        if (tarefa->ativa && tarefa->deadline_absoluto == tempo) {
            tarefa->ativa = 0;
            tarefa->restante = 0;
            tarefa->perdidas++;
        }
    }
}
void criar_instancias(Simulacao *sim, int tempo) {
    int i;

    for (i = 0; i < sim->quantidade; i++) {
        Tarefa *tarefa = &sim->tarefas[i];

        tarefa->ativa = 1;
        tarefa->restante = tarefa->burst;
        tarefa->chegada = tempo;
        tarefa->deadline_absoluto = tempo + tarefa->deadline;
        tarefa->liberadas++;
    }
}

int salvar_saida(
    const Simulacao *sim,
    Algoritmo algoritmo,
    const char *nome_saida,
    const Historico *hist
) {
    FILE *saida = fopen(nome_saida, "w");
    int i;

    if (!saida) {
        fprintf(stderr,"Erro: nao foi possivel criar '%s'.\n",nome_saida);
        return 0;
    }

    fprintf(saida,"EXECUTION BY %s\n",algoritmo == RATE ? "RATE" : "EDF");

    for (i = 0; i < hist->qtd_trechos; i++) {
        const Trecho *trecho = &hist->trechos[i];
        int duracao = trecho->fim - trecho->inicio;

        if (trecho->tarefa == -1) {
            fprintf(saida,"idle for %d units\n",duracao);
        } 
        else {
            fprintf(saida,"[%s] for %d units - %c\n",sim->tarefas[trecho->tarefa].nome,duracao,trecho->motivo);
        }
    }

    fprintf(saida, "LOST DEADLINES\n");

    for (i = 0; i < sim->quantidade; i++) {
        fprintf(saida,"[%s] %d\n",sim->tarefas[i].nome,sim->tarefas[i].perdidas);
    }

    fprintf(saida, "COMPLETE EXECUTION\n");

    for (i = 0; i < sim->quantidade; i++) {
        fprintf(saida,"[%s] %d\n",sim->tarefas[i].nome,sim->tarefas[i].concluidas);
    }

    fprintf(saida, "KILLED\n");

    for (i = 0; i < sim->quantidade; i++) {
        fprintf(saida,"[%s] %d\n",sim->tarefas[i].nome,sim->tarefas[i].encerradas);
    }
    if (fclose(saida)) {
        fprintf(stderr,"Erro ao finalizar '%s'.\n",nome_saida);
    return 0;
    }

return 1;
}

int executar(
    Simulacao *sim,
    Algoritmo algoritmo,
    const char *nome_saida
) {
    Historico hist = {0};
    int erro_memoria = 0;
    int tempo;

    for (tempo = 0;
         tempo < sim->tempo_total && !erro_memoria;
         tempo++) {
        int escolhida;
        Tarefa *rodando;

        checar_prazos(sim, tempo);
        criar_instancias(sim, tempo);

        escolhida = escolher_tarefa(sim, algoritmo);

        if (!guardar_trecho(&hist, tempo, escolhida)) {
            erro_memoria = 1;
            break;
        }

        if (escolhida == -1) {
            continue;
        }

        rodando = &sim->tarefas[escolhida];

        rodando->restante--;
        rodando->tempo_cpu++;

        if (!rodando->restante) {
            rodando->ativa = 0;
            rodando->concluidas++;

            hist.trechos[hist.qtd_trechos - 1].motivo = 'F';
        }
    }

    if (erro_memoria) {
        fprintf(stderr,"Erro: memoria insuficiente durante a simulacao.\n");
        free(hist.trechos);
        return 0;
    }

    free(hist.trechos);

    (void) nome_saida;

    return 1;
}