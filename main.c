#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void tela_menu_principal() {

    system("clear||cls");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║ SIG-BURGER - SISTEMA DE GESTÃO DE HAMBURGUERIA   ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Módulo Clientes                             ║\n");
    printf("║ ► 2. Módulo Pedidos                              ║\n");
    printf("║ ► 3. Módulo Cardápio                             ║\n");
    printf("║ ► 4. Módulo Estoque                              ║\n");
    printf("║ ► 5. Módulo Financeiro                           ║\n");
    printf("║ ► 6. Módulo Relatórios                           ║\n");
    printf("║ ► 7. sobre o SIG-Burger                          ║\n");
    printf("║ ► 0. Sair do Sistema                             ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");

}



int main(void) {
    int opcao;

    do {
        tela_menu_principal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Módulo Clientes selecionado.\n");
                // Chamar função do módulo Clientes
                break;
            case 2:
                printf("Módulo Pedidos selecionado.\n");
                // Chamar função do módulo Pedidos
                break;
            case 3:
                printf("Módulo Cardápio selecionado.\n");
                // Chamar função do módulo Cardápio
                break;
            case 4:
                printf("Módulo Estoque selecionado.\n");
                // Chamar função do módulo Estoque
                break;
            case 5:
                printf("Módulo Financeiro selecionado.\n");
                // Chamar função do módulo Financeiro
                break;
            case 6:
                printf("Módulo Relatórios selecionado.\n");
                // Chamar função do módulo Relatórios
                break;
            case 7:
                printf("Sobre o SIG-Burger selecionado.\n");
                // Chamar função sobre o SIG-Burger
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("Pressione Enter para continuar...");
            while (getchar() != '\n'); // Limpa o buffer
            getchar(); // Espera o Enter
        }

    } while (opcao != 0);
    return 0;
}
