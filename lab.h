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

size_t meu_strlen(const char *str);
void removerQuebraLinha(char *str);
void cadastrarTarefa(FILE *arquivo);
void deletarTarefa(FILE *arquivo);
void listarTarefas(FILE *arquivo);
void encerrarPrograma(FILE *arquivo);


#endif // LAB_H




