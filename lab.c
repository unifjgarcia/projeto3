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

// Função para alterar uma tarefa no arquivo com base na prioridade
void alterarTarefa(FILE *arquivo) {
    int prioridade;
    printf("Digite a prioridade da tarefa que deseja alterar: ");
    scanf("%d", &prioridade);

    fseek(arquivo, 0, SEEK_SET);

    struct Tarefa tarefa;
    int encontrou = 0;

    FILE *temporario = fopen("temporario.dat", "wb");
    if (temporario == NULL) {
        perror("Erro ao criar arquivo temporário");
        return;
    }

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (tarefa.prioridade == prioridade) {
            encontrou = 1;
            printf("Tarefa encontrada. Escolha o campo que deseja alterar:\n");
            printf("1. Prioridade\n");
            printf("2. Descricao\n");
            printf("3. Categoria\n");
            printf("4. Estado\n");

            int opcaoCampo;
            scanf("%d", &opcaoCampo);

            switch (opcaoCampo) {
                case 1:
                    printf("Digite a nova prioridade: ");
                    scanf("%d", &tarefa.prioridade);
                    break;
                case 2:
                    printf("Digite a nova descricao: ");
                    scanf(" %[^\n]s", tarefa.descricao);
                    break;
                case 3:
                    printf("Digite a nova categoria: ");
                    scanf(" %[^\n]s", tarefa.categoria);
                    break;
                case 4:
                    printf("Escolha o novo estado:\n");
                    printf("1. Completo\n");
                    printf("2. Em andamento\n");
                    printf("3. Nao iniciado\n");

                    int opcaoEstado;
                    scanf("%d", &opcaoEstado);

                    if (opcaoEstado >= 1 && opcaoEstado <= 3) {
                        snprintf(tarefa.estado, sizeof(tarefa.estado), "%s",
                                 (opcaoEstado == 1) ? "Completo" :
                                 (opcaoEstado == 2) ? "Em andamento" : "Nao iniciado");
                    } else {
                        printf("Opcao invalida. O estado da tarefa permanecera o mesmo.\n");
                    }
                    break;
                default:
                    printf("Opcao invalida. Nenhum campo foi alterado.\n");
                    break;
            }
        }

        fwrite(&tarefa, sizeof(struct Tarefa), 1, temporario);
    }

    fclose(arquivo);
    fclose(temporario);

    if (!encontrou) {
        remove("temporario.dat");
        printf("Tarefa com prioridade %d nao encontrada. Nenhum campo foi alterado.\n", prioridade);
    } else {
        remove("tarefas.dat");
        rename("temporario.dat", "tarefas.dat");
        printf("Tarefa com prioridade %d alterada com sucesso!\n", prioridade);
    }

    arquivo = fopen("tarefas.dat", "rb+");
    if (arquivo == NULL) {
        perror("Erro ao reabrir o arquivo de tarefas");
        return;
    }
}

// Função para filtrar tarefas por prioridade
void filtrarPorPrioridade(FILE *arquivo) {
    int prioridade;
    printf("Digite a prioridade desejada: ");
    scanf("%d", &prioridade);

    fseek(arquivo, 0, SEEK_SET);

    struct Tarefa tarefa;
    int encontrou = 0;

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (tarefa.prioridade == prioridade) {
            encontrou = 1;
            printf("Prioridade: %d\n", tarefa.prioridade);
            printf("Descricao: %s\n", tarefa.descricao);
            printf("Categoria: %s\n", tarefa.categoria);
            printf("Estado: %s\n", tarefa.estado);
            printf("\n");
        }
    }

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada com a prioridade %d.\n", prioridade);
    }
}

// Função auxiliar para comparar estados de tarefas
int compararEstados(const char *estado1, const char *estado2) {
    while (*estado1 != '\0' && *estado2 != '\0') {
        if (*estado1 != *estado2) {
            return 0;  // Caracteres diferentes, estados diferentes
        }
        estado1++;
        estado2++;
    }

    // Chegou ao final de ambos os estados
    return (*estado1 == '\0' && *estado2 == '\0');
}

// Função para filtrar tarefas por estado
void filtrarPorEstado(FILE *arquivo) {
    char estado[20];
    printf("Digite o estado desejado (Completo, Em andamento, Nao iniciado, Indeterminado): ");
    scanf(" %[^\n]s", estado);

    fseek(arquivo, 0, SEEK_SET);

    struct Tarefa tarefa;
    int encontrou = 0;

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (compararEstados(tarefa.estado, estado)) {
            encontrou = 1;
            printf("Prioridade: %d\n", tarefa.prioridade);
            printf("Descricao: %s\n", tarefa.descricao);
            printf("Categoria: %s\n", tarefa.categoria);
            printf("Estado: %s\n", tarefa.estado);
            printf("\n");
        }
    }

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada com o estado %s.\n", estado);
    }
}

// Função auxiliar para comparar categorias de tarefas
int compararCategorias(const char *categoria1, const char *categoria2) {
    while (*categoria1 != '\0' && *categoria2 != '\0') {
        if (*categoria1 != *categoria2) {
            return 0;  // Caracteres diferentes, categorias diferentes
        }
        categoria1++;
        categoria2++;
    }

    // Chegou ao final de ambas as categorias
    return (*categoria1 == '\0' && *categoria2 == '\0');
}

// Função para filtrar tarefas por categoria
void filtrarPorCategoria(FILE *arquivo) {
    char categoria[100];
    printf("Digite a categoria desejada: ");
    scanf(" %[^\n]s", categoria);

    fseek(arquivo, 0, SEEK_SET);

    struct Tarefa tarefa;
    int encontrou = 0;

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (compararCategorias(tarefa.categoria, categoria)) {
            encontrou = 1;
            printf("Prioridade: %d\n", tarefa.prioridade);
            printf("Descricao: %s\n", tarefa.descricao);
            printf("Categoria: %s\n", tarefa.categoria);
            printf("Estado: %s\n", tarefa.estado);
            printf("\n");
        }
    }

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada com a categoria %s.\n", categoria);
    }
}

// Função para filtrar tarefas por prioridade e categoria
void filtrarPorPrioridadeECategoria(FILE *arquivo) {
    int prioridade;
    printf("Digite a prioridade desejada: ");
    scanf("%d", &prioridade);

    char categoria[100];
    printf("Digite a categoria desejada: ");
    scanf(" %[^\n]s", categoria);

    fseek(arquivo, 0, SEEK_SET);

    struct Tarefa tarefa;
    int encontrou = 0;

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (tarefa.prioridade == prioridade && compararCategorias(tarefa.categoria, categoria)) {
            encontrou = 1;
            printf("Prioridade: %d\n", tarefa.prioridade);
            printf("Descricao: %s\n", tarefa.descricao);
            printf("Categoria: %s\n", tarefa.categoria);
            printf("Estado: %s\n", tarefa.estado);
            printf("\n");
        }
    }

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada com a prioridade %d e categoria %s.\n", prioridade, categoria);
    }
}

// Função para exportar tarefas por prioridade para um arquivo
void exportarPorPrioridade(FILE *arquivo) {
    int prioridade;
    printf("Digite a prioridade desejada para exportar: ");
    scanf("%d", &prioridade);

    fseek(arquivo, 0, SEEK_SET);

    FILE *arquivoExportado = fopen("ExportaPrioridade.txt", "w");
    if (arquivoExportado == NULL) {
        perror("Erro ao criar o arquivo exportado");
        return;
    }

    struct Tarefa tarefa;

    while (fread(&tarefa, sizeof(struct Tarefa), 1, arquivo) == 1) {
        if (tarefa.prioridade == prioridade) {
            fprintf(arquivoExportado, "%d, %s, %s, %s\n", tarefa.prioridade, tarefa.categoria, tarefa.estado, tarefa.descricao);
        }
    }

    fclose(arquivoExportado);

    printf("Tarefas da prioridade %d exportadas para o arquivo ExportaPrioridade.txt com sucesso!\n", prioridade);
}


// Função para exportar tarefas por categoria para um arquivo
void exportarPorCategoria(FILE *arquivo) {
    char categoria[100];
    printf("Digite a categoria desejada para exportar: ");
    scanf(" %[^\n]s", categoria);

    fseek(arquivo, 0, SEEK_SET);

    FILE *arquivoExportado = fopen("ExportaCategoria.txt", "w");
    if (arquivoExportado == NULL) {
        perror("Erro ao criar o arquivo exportado");
        return;
    }

    struct Tarefa tarefas[1000];
    int quantidadeTarefas = 0;

    while (fread(&tarefas[quantidadeTarefas], sizeof(struct Tarefa), 1, arquivo) == 1) {
        quantidadeTarefas++;
    }

    // Ordenar as tarefas por prioridade (usando Bubble Sort para simplicidade)
    for (int i = 0; i < quantidadeTarefas - 1; i++) {
        for (int j = 0; j < quantidadeTarefas - i - 1; j++) {
            if (tarefas[j].prioridade > tarefas[j + 1].prioridade) {
                // Trocar as tarefas de lugar
                struct Tarefa temp = tarefas[j];
                tarefas[j] = tarefas[j + 1];
                tarefas[j + 1] = temp;
            }
        }
    }

    int encontrou = 0;

    for (int i = 0; i < quantidadeTarefas; i++) {
        if (compararCategorias(tarefas[i].categoria, categoria)) {
            encontrou = 1;
            fprintf(arquivoExportado, "%d, %s, %s, %s\n", tarefas[i].prioridade, tarefas[i].categoria, tarefas[i].estado, tarefas[i].descricao);
        }
    }

    fclose(arquivoExportado);

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada na categoria %s.\n", categoria);
    } else {
        printf("Tarefas na categoria %s exportadas para o arquivo ExportaCategoria.txt com sucesso!\n", categoria);
    }
}

// Função para exportar tarefas por prioridade e categoria para um arquivo
void exportarPorPrioridadeECategoria(FILE *arquivo) {
    char categoria[100];
    printf("Digite a categoria desejada para exportar: ");
    scanf(" %[^\n]s", categoria);

    int prioridade;
    printf("Digite a prioridade desejada para exportar: ");
    scanf("%d", &prioridade);

    fseek(arquivo, 0, SEEK_SET);

    FILE *arquivoExportado = fopen("ExportaPrioridadeCategoria.txt", "w");
    if (arquivoExportado == NULL) {
        perror("Erro ao criar o arquivo exportado");
        return;
    }

    struct Tarefa tarefas[1000];
    int quantidadeTarefas = 0;

    while (fread(&tarefas[quantidadeTarefas], sizeof(struct Tarefa), 1, arquivo) == 1) {
        quantidadeTarefas++;
    }

    // Ordenar as tarefas por prioridade (usando Bubble Sort para simplicidade)
    for (int i = 0; i < quantidadeTarefas - 1; i++) {
        for (int j = 0; j < quantidadeTarefas - i - 1; j++) {
            if (tarefas[j].prioridade > tarefas[j + 1].prioridade) {
                // Trocar as tarefas de lugar
                struct Tarefa temp = tarefas[j];
                tarefas[j] = tarefas[j + 1];
                tarefas[j + 1] = temp;
            }
        }
    }

    int encontrou = 0;

    for (int i = 0; i < quantidadeTarefas; i++) {
        if (compararCategorias(tarefas[i].categoria, categoria) && tarefas[i].prioridade == prioridade) {
            encontrou = 1;
            fprintf(arquivoExportado, "%d, %s, %s, %s\n", tarefas[i].prioridade, tarefas[i].categoria, tarefas[i].estado, tarefas[i].descricao);
        }
    }

    fclose(arquivoExportado);

    if (!encontrou) {
        printf("Nenhuma tarefa encontrada na categoria %s com prioridade %d.\n", categoria, prioridade);
    } else {
        printf("Tarefas na categoria %s com prioridade %d exportadas para o arquivo ExportaPrioridadeCategoria.txt com sucesso!\n", categoria, prioridade);
    }
}

// Função para encerrar o programa, fechando o arquivo
void encerrarPrograma(FILE *arquivo) {
    fclose(arquivo);
    printf("Voce saiu!\n");
}
