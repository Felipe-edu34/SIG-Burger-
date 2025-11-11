#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void menu_cardapio() {

    system("clear||cls");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                  MÓDULO CARDÁPIO                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Adicionar Item ao Cardápio                  ║\n");
    printf("║ ► 2. Remover Item do Cardápio                    ║\n");
    printf("║ ► 3. Atualizar Item do Cardápio                  ║\n");
    printf("║ ► 4  pesquisar Item do cardapio                  ║\n");
    printf("║ ► 5. Visualizar Cardápio                         ║\n");
    printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n"); 
    printf("Escolha uma opção: ");

}



void cardapio() {
    int opcao;

    do {
        menu_cardapio();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Adicionar Item ao Cardápio selecionado.\n");
                // Chamar função para adicionar item
                break;
            case 2:
                printf("Remover Item do Cardápio selecionado.\n");
                // Chamar função para remover item
                break;
            case 3:
                printf("Atualizar Item do Cardápio selecionado.\n");
                // Chamar função para atualizar item
                break;
            case 4:
                printf("Pesquisar Item do Cardápio selecionado.\n");
                // Chamar função para pesquisar item
                break;
            case 5:
                printf("Visualizar Cardápio selecionado.\n");
                // Chamar função para visualizar cardápio
                break;
            case 0:
                printf("Voltando ao Menu Principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        system("pause");

    } while (opcao != 0);
}