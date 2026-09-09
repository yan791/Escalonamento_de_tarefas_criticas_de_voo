#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_NAME 31

typedef enum {
    RATE,
    EDF
} Algoritmo;

typedef struct {
    char nome[MAX_NAME + 1];
    int periodo;
    int deadline;
    int burst;

    int restante;
    int chegada;
    int deadline_absoluto;
    int ativa;

    int liberadas;
    int concluidas;
    int perdidas;
    int encerradas;
    int tempo_cpu;
} Tarefa;

typedef struct {
    int tempo_total;
    int quantidade;
    Tarefa *tarefas;
} Simulacao;

typedef struct {
    int inicio;
    int fim;
    int tarefa;
    char motivo;
} Trecho;

typedef struct {
    Trecho *trechos;
    int qtd_trechos;
    int cap_trechos;
} Historico;

int ler_entrada(const char *nome_arquivo, Simulacao *sim);
void liberar_dados(Simulacao *sim);

int executar(
    Simulacao *sim,
    Algoritmo algoritmo,
    const char *nome_saida
);

#endif