#ifndef LAB_H
#define LAB_H

#include <stdio.h>

// criação da nossa struct com as informações basicas para nosso projeto
struct Tarefa {
    int prioridade;
    char descricao[300];
    char categoria[100];
    char estado[20];
};

// Chamada das funções utilizadas no código
size_t meu_strlen(const char *str);
void removerQuebraLinha(char *str);
void cadastrarTarefa(FILE *arquivo);
void deletarTarefa(FILE *arquivo);
void listarTarefas(FILE *arquivo);
void alterarTarefa(FILE *arquivo);
void filtrarPorPrioridade(FILE *arquivo);
int compararEstados(const char *estado1, const char *estado2);
void filtrarPorEstado(FILE *arquivo);
int compararCategorias(const char *categoria1, const char *categoria2);
void filtrarPorCategoria(FILE *arquivo);
void filtrarPorPrioridadeECategoria(FILE *arquivo);
void encerrarPrograma(FILE *arquivo);


#endif // LAB_H




