#include "lab.h"
#include <stdio.h>

size_t meu_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void removerQuebraLinha(char *str) {
    size_t len = meu_strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void cadastrarTarefa(FILE *arquivo) {
    struct Tarefa tarefa;

    printf("Digite a prioridade da tarefa (entre 0 e 10): ");
    scanf("%d", &tarefa.prioridade);
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    printf("Digite a descricao da tarefa (ate 300 caracteres): ");
    fgets(tarefa.descricao, sizeof(tarefa.descricao), stdin);
    removerQuebraLinha(tarefa.descricao);

    printf("Digite a categoria da tarefa (ate 100 caracteres): ");
    fgets(tarefa.categoria, sizeof(tarefa.categoria), stdin);
    removerQuebraLinha(tarefa.categoria);

    // Escrever a tarefa no arquivo
    fwrite(&tarefa, sizeof(struct Tarefa), 1, arquivo);

    printf("Tarefa cadastrada com sucesso!\n");
}

void deletarTarefa(FILE *arquivo) {
    struct Tarefa tarefa;
    int encontrou = 0;  // Flag para verificar se a tarefa foi encontrada

    int prioridade;
    printf("Digite a prioridade da tarefa que deseja deletar: ");
    scanf("%d", &prioridade);

    // Move o ponteiro de leitura para o início do arquivo
    fseek(arquivo, 0, SEEK_SET);

    // Abre um arquivo temporário para escrita
    FILE *temporario = fopen("temporario.dat", "wb");
    if (temporario == NULL) {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    // Lê as tarefas do arquivo original e escreve no arquivo temporário, exceto a tarefa a ser deletada
    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (tarefa.prioridade == prioridade) {
            encontrou = 1;
            printf("Tarefa encontrada e deletada:\n");
            printf("Prioridade: %d\n", tarefa.prioridade);
            printf("Descricao: %s\n", tarefa.descricao);
            printf("Categoria: %s\n", tarefa.categoria);
            printf("\n");
        } else {
            // Escrever todas as tarefas que não correspondem à prioridade no arquivo temporário
            fwrite(&tarefa, sizeof(struct Tarefa), 1, temporario);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrou) {
        remove("temporario.dat");
        printf("Tarefa com prioridade %d não encontrada. Nenhuma tarefa foi deletada.\n", prioridade);
    } else {
        remove("tarefas.dat");
        rename("temporario.dat", "tarefas.dat");
        printf("Tarefa com prioridade %d deletada com sucesso!\n", prioridade);
    }

    // Reabre o arquivo para continuar usando
    arquivo = fopen("tarefas.dat", "rb+");
    if (arquivo == NULL) {
        perror("Erro ao reabrir o arquivo de tarefas");
        return;
    }
}

void listarTarefas(FILE *arquivo) {
    struct Tarefa tarefa;

    // Move o ponteiro de leitura para o início do arquivo
    fseek(arquivo, 0, SEEK_SET);

    printf("Lista de Tarefas:\n");

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        printf("Prioridade: %d\n", tarefa.prioridade);
        printf("Descricao: %s\n", tarefa.descricao);
        printf("Categoria: %s\n", tarefa.categoria);
        printf("\n");
    }
}

void encerrarPrograma(FILE *arquivo) {
    fclose(arquivo);
    printf("Voce saiu!\n");
}
