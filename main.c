#include <stdio.h>
#include "lab.h"

int main() {
    // Abre o arquivo de tarefas no modo de leitura e escrita binária
    FILE *arquivo = fopen("tarefas.dat", "ab+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de tarefas");
        return 1;
    }
    // Variável para armazenar a opção do usuário no menu
    int opcao;
    // Loop do menu principal
    do {
        printf("Menu:\n");
        printf("1. Cadastrar Tarefa\n");
        printf("2. Listar Tarefas\n");
        printf("3. Deletar Tarefa\n");
        printf("4. Alterar Tarefa\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        // Recebe a opção escolhida pelo usuário   
        scanf("%d", &opcao);
        // Executa a operação correspondente à opção escolhida
        switch (opcao) {
            case 1:
                cadastrarTarefa(arquivo);
                break;
            case 2:
                listarTarefas(arquivo);
                break;
            case 3:
                deletarTarefa(arquivo);
                break;
            case 4:
                alterarTarefa(arquivo);
                break;
            case 5:
                encerrarPrograma(arquivo);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);
    // Retorna 0 indicando que o programa foi executado com sucesso
    return 0;
}
