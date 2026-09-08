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
} Tarefa;

typedef struct {
    int tempo_total;
    int quantidade;
    Tarefa *tarefas;
} Simulacao;

int ler_entrada(const char *nome_arquivo, Simulacao *sim);
void liberar_dados(Simulacao *sim);

#endif