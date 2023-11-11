#include "lab.h"
#include <stdio.h>

// Implementação da função meu_strlen para calcular o comprimento de uma string
size_t meu_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
// Implementação da função removerQuebraLinha para remover o caractere de nova linha de uma string
void removerQuebraLinha(char *str) {
    size_t len = meu_strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Função para cadastrar uma nova tarefa no arquivo
void cadastrarTarefa(FILE *arquivo) {
    struct Tarefa tarefa;

    // Solicitação e leitura da prioridade da tarefa
    printf("Digite a prioridade da tarefa (entre 0 e 10): ");
    scanf("%d", &tarefa.prioridade);
    getchar();

    // Solicitação e leitura da descrição da tarefa
    printf("Digite a descricao da tarefa (ate 300 caracteres): ");
    fgets(tarefa.descricao, sizeof(tarefa.descricao), stdin);
    removerQuebraLinha(tarefa.descricao);

    // Solicitação e leitura da categoria da tarefa
    printf("Digite a categoria da tarefa (ate 100 caracteres): ");
    fgets(tarefa.categoria, sizeof(tarefa.categoria), stdin);
    removerQuebraLinha(tarefa.categoria);

    // Solicitação do estado da tarefa
    printf("Escolha o estado da tarefa:\n");
    printf("1. Completo\n");
    printf("2. Em andamento\n");
    printf("3. Nao iniciado\n");
    printf("Digite o numero correspondente ao estado desejado: ");
    int opcaoEstado;
    scanf("%d", &opcaoEstado);

    // Atualizar o estado da tarefa
    if (opcaoEstado == 1) {
        for (int i = 0; i < sizeof(tarefa.estado) - 1; ++i) {
            tarefa.estado[i] = "Completo"[i];
        }
    } else if (opcaoEstado == 2) {
        for (int i = 0; i < sizeof(tarefa.estado) - 1; ++i) {
            tarefa.estado[i] = "Em andamento"[i];
        }
    } else if (opcaoEstado == 3) {
        for (int i = 0; i < sizeof(tarefa.estado) - 1; ++i) {
            tarefa.estado[i] = "Nao iniciado"[i];
        }
    } else {
        printf("Opcao invalida. O estado da tarefa sera definido como 'Indeterminado'.\n");
        for (int i = 0; i < sizeof(tarefa.estado) - 1; ++i) {
            tarefa.estado[i] = "Indeterminado"[i];
        }
    }

    // Escrever a tarefa no arquivo
    fwrite(&tarefa, sizeof(struct Tarefa), 1, arquivo);

    printf("Tarefa cadastrada com sucesso!\n");

}

// Função para deletar uma tarefa do arquivo com base na prioridade
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
            printf("Estado: %s\n", tarefa.estado);
            printf("\n");
        } else {
            // Escrever todas as tarefas que não correspondem à prioridade no arquivo temporário
            fwrite(&tarefa, sizeof(struct Tarefa), 1, temporario);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    // Verificação se a tarefa foi encontrada e removida ou não
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

// Função para listar todas as tarefas presentes no arquivo
void listarTarefas(FILE *arquivo) {
    struct Tarefa tarefa;

    // Move o ponteiro de leitura para o início do arquivo
    fseek(arquivo, 0, SEEK_SET);

    printf("Lista de Tarefas:\n");

    // Leitura e exibição de todas as tarefas no arquivo
    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        printf("Prioridade: %d\n", tarefa.prioridade);
        printf("Descricao: %s\n", tarefa.descricao);
        printf("Categoria: %s\n", tarefa.categoria);
        printf("Estado: %s\n", tarefa.estado);
        printf("\n");
    }
}

// Função para encerrar o programa, fechando o arquivo
void encerrarPrograma(FILE *arquivo) {
    fclose(arquivo);
    printf("Voce saiu!\n");
}
