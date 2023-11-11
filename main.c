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
        printf("5. Filtrar por Prioridade\n");
        printf("6. Filtrar por Estado\n");
        printf("7. Filtrar por Categoria\n");
        printf("8. Filtrar por Prioridade e Categoria\n");
        printf("9. Exportar por Prioridade\n");
        printf("10. Exportar por Categoria\n");
        printf("11. Exportar por Prioridade e Categoria\n");
        printf("12. Sair\n");
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
                filtrarPorPrioridade(arquivo);
                break;
            case 6:
                filtrarPorEstado(arquivo);
                break;
            case 7:
                filtrarPorCategoria(arquivo);
                break;
            case 8:
                filtrarPorPrioridadeECategoria(arquivo);
                break;
            case 9:
                exportarPorPrioridade(arquivo);
                break;
            case 10:
                exportarPorCategoria(arquivo);
                break;
            case 11:
                exportarPorPrioridadeECategoria(arquivo);
                break;
            case 12:
                encerrarPrograma(arquivo);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 12);
    // Retorna 0 indicando que o programa foi executado com sucesso
    return 0;
}
