# Escalonamento de Tarefas Críticas de Voo

Projeto desenvolvido em C para simular o escalonamento preemptivo de tarefas periódicas críticas de voo.

O programa implementa dois algoritmos de escalonamento:

- **Rate Monotonic (`rate`)**: a tarefa com o menor período possui maior prioridade;
- **Earliest Deadline First (`edf`)**: a tarefa com o menor deadline absoluto possui maior prioridade.

Em caso de empate, permanece a tarefa que aparece primeiro no arquivo de entrada.

## Estrutura do projeto

```text
yrn/
├── include/
│   └── scheduler.h
├── src/
│   ├── input.c
│   ├── main.c
│   └── simulation.c
├── .gitignore
├── Makefile
├── README.md
└── evidencias.log
```

### Descrição dos arquivos

- `include/scheduler.h`: contém as estruturas, tipos e funções públicas do projeto;
- `src/input.c`: realiza a leitura e a validação do arquivo de entrada;
- `src/simulation.c`: implementa os algoritmos e controla a simulação;
- `src/main.c`: valida os argumentos e inicia o programa;
- `Makefile`: contém as regras de compilação e limpeza;
- `evidencias.log`: registra o histórico de commits do desenvolvimento.

## Requisitos

O projeto utiliza o padrão C11 e foi desenvolvido para sistemas Linux, Unix e macOS.

Para compilar, é necessário possuir:

- compilador C compatível com C11, como GCC ou Clang;
- programa `make`.

## Compilação

Na raiz do projeto, execute:

```bash
make clean
make
```

O comando criará um executável chamado:

```text
scheduler
```

A compilação utiliza as seguintes opções:

```text
-std=c11 -Wall -Wextra -Wpedantic
```

Essas opções ativam o padrão C11 e verificações adicionais do compilador.

## Execução

O programa deve ser executado utilizando:

```text
./scheduler <rate|edf> <arquivo_de_entrada>
```

### Rate Monotonic

```bash
./scheduler rate entrada.txt
```

O resultado será gravado em:

```text
rate_yrn.out
```

### Earliest Deadline First

```bash
./scheduler edf entrada.txt
```

O resultado será gravado em:

```text
edf_yrn.out
```

Durante uma execução válida, o resultado não é exibido diretamente no terminal. Ele é gravado no arquivo correspondente ao algoritmo selecionado.

## Formato do arquivo de entrada

A primeira linha informa o tempo total da simulação:

```text
TEMPO_TOTAL
```

As linhas seguintes representam as tarefas:

```text
NOME PERIODO DEADLINE BURST
```

Exemplo:

```text
20
NAV 5 5 2
COM 10 8 3
```

Nesse exemplo:

- a simulação possui duração de 20 unidades de tempo;
- `NAV` possui período 5, deadline 5 e burst 2;
- `COM` possui período 10, deadline 8 e burst 3.

## Regras de validação

Todos os valores numéricos devem ser inteiros positivos.

Cada tarefa precisa respeitar:

```text
BURST <= DEADLINE <= PERIODO
```

Isso significa que:

- o burst não pode ser maior que o deadline;
- o deadline não pode ser maior que o período;
- período, deadline e burst precisam ser maiores que zero.

O nome de uma tarefa pode possuir até 31 caracteres.

Linhas vazias entre as tarefas são ignoradas.

O programa rejeita:

- arquivos inexistentes;
- arquivos vazios;
- tempo total inválido;
- tarefas com campos ausentes;
- tarefas com campos excedentes;
- valores iguais ou menores que zero;
- arquivos sem tarefas;
- relações que não respeitem `BURST <= DEADLINE <= PERIODO`.

## Estruturas principais

### Tarefa

A estrutura `Tarefa` armazena os dados fixos da tarefa:

- nome;
- período;
- deadline;
- burst.

Ela também armazena os dados utilizados durante a simulação:

- tempo restante;
- instante de chegada;
- deadline absoluto;
- estado ativo.

Por fim, mantém os contadores:

- instâncias liberadas;
- execuções concluídas;
- deadlines perdidos;
- tarefas encerradas;
- tempo total de CPU.

### Simulação

A estrutura `Simulacao` guarda:

- tempo total;
- quantidade de tarefas;
- vetor dinâmico de tarefas.

### Histórico

O histórico utiliza um vetor dinâmico de trechos.

Cada trecho informa:

- instante inicial;
- instante final;
- índice da tarefa;
- motivo do encerramento.

## Funcionamento da simulação

Em cada unidade de tempo, o simulador realiza as operações nesta ordem:

1. verifica se alguma tarefa atingiu o deadline sem concluir;
2. libera novas instâncias conforme o período;
3. seleciona a tarefa ativa com maior prioridade;
4. executa uma unidade de CPU;
5. verifica se a tarefa terminou.

Uma tarefa que conclui sua execução exatamente no deadline é considerada concluída.

Um deadline igual ao tempo total também é verificado. Depois dessa verificação, as tarefas que ainda estiverem ativas são encerradas pelo fim da simulação.

## Rate Monotonic

No Rate Monotonic, a prioridade é definida pelo período:

```text
menor período = maior prioridade
```

Exemplo:

```text
TAREFA_A 5 5 2
TAREFA_B 10 8 3
```

A tarefa `TAREFA_A` possui maior prioridade porque seu período é menor.

## Earliest Deadline First

No EDF, a prioridade é definida pelo deadline absoluto:

```text
menor deadline absoluto = maior prioridade
```

O deadline absoluto é calculado por:

```text
deadline absoluto = instante de chegada + deadline relativo
```

Como o deadline absoluto muda a cada nova instância, a prioridade no EDF pode mudar durante a simulação.

## Preempção

A simulação é preemptiva.

Isso significa que uma tarefa em execução pode perder a CPU quando outra tarefa com prioridade maior for liberada.

A tarefa preemptada continua ativa e poderá retornar à CPU posteriormente, desde que não perca o deadline.

## Histórico da CPU

O simulador agrupa unidades consecutivas da mesma tarefa em um único trecho.

Por exemplo, uma tarefa que utiliza a CPU do tempo 2 até o tempo 5 será representada pelo intervalo:

```text
[2, 5)
```

Esse intervalo inclui os tempos 2, 3 e 4, totalizando três unidades.

Quando nenhuma tarefa está disponível para execução, o histórico registra:

```text
idle
```

## Motivos dos trechos

Cada trecho executado possui um caractere indicando por que ele terminou:

| Código | Significado |
|:------:|-------------|
| `F` | A tarefa concluiu sua execução |
| `H` | A tarefa foi preemptada, mas continua ativa |
| `L` | A tarefa perdeu o deadline |
| `K` | A simulação terminou com a tarefa ainda ativa |

Trechos ociosos são apresentados como `idle` e não possuem código de encerramento.

## Formato da saída

O arquivo começa identificando o algoritmo utilizado:

```text
EXECUTION BY RATE
```

ou:

```text
EXECUTION BY EDF
```

Depois, apresenta o histórico da CPU:

```text
[NAV] for 2 units - F
[COM] for 3 units - F
idle for 2 units
```

Ao final, são exibidas três seções de resumo.

### Deadlines perdidos

```text
LOST DEADLINES
[NAV] 0
[COM] 1
```

Essa seção informa quantas instâncias de cada tarefa perderam o deadline.

### Execuções concluídas

```text
COMPLETE EXECUTION
[NAV] 4
[COM] 2
```

Essa seção informa quantas instâncias foram concluídas.


Compile o projeto:

```bash
make clean
make
```

Execute o Rate Monotonic:

```bash
./scheduler rate entrada.txt
```

Visualize o resultado:

```bash
cat rate_yrn.out
```

Execute o EDF:

```bash
./scheduler edf entrada.txt
```

Visualize o resultado:

```bash
cat edf_yrn.out
```

## Tratamento de erros

Se a quantidade de argumentos estiver incorreta, o programa exibe:

```text
Uso: ./scheduler <rate|edf> <arquivo_de_entrada>
```

Se o algoritmo informado for inválido:

```text
Erro: algoritmo invalido 'outro'. Use rate ou edf.
```

Se o arquivo de entrada não existir:

```text
Erro: nao foi possivel abrir 'arquivo.txt'.
```

As mensagens de erro são enviadas para `stderr`, e o programa retorna um código diferente de zero.

## Limpeza

Para remover o executável, os arquivos objeto e as saídas geradas, execute:

```bash
make clean
```

O comando remove:

```text
src/main.o
src/input.o
src/simulation.o
scheduler
rate_yrn.out
edf_yrn.out
```
