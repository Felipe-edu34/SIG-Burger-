#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "relatorio.h"
#include "cliente.h"
#include "cardapio.h"

void menu_relatorio(){
    limpar_tela();
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║               MÓDULO DE RELATÓRIOS               ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║                                                  ║\n");
        printf("║ ► 1. Relatório de cardapio                       ║\n");
        printf("║ ► 2. Relatório de Estoque                        ║\n");
        printf("║ ► 3. Relatório de Clientes                       ║\n");
        printf("║ ► 4. Relatório de pedidos                        ║\n");
        printf("║ ► 5. Relatorio financeiro                        ║\n");
        printf("║                                                  ║\n");
        printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
        printf("║                                                  ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        printf("Escolha uma opção: ");
}



void relatorio() {
    int opcao;

    do {
        menu_relatorio();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("Relatório de Cardápio selecionado.\n");
                // Chamar função do módulo Cardápio
                break;
            case 2:
                printf("Relatório de Estoque selecionado.\n");
                // Chamar função do módulo Estoque
                break;
            case 3:
                printf("Relatório de Clientes selecionado.\n");
                // Chamar função do módulo Clientes
                break;
            case 4:
                printf("Relatório de Pedidos selecionado.\n");
                // Chamar função do módulo Pedidos
                break;
            case 5:
                printf("Relatório Financeiro selecionado.\n");
                // Chamar função do módulo Financeiro
                break;
            case 0:
                printf("\nVoltando ao Menu Principal...\n");
                pausar();
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                pausar();
                break;
        }
    } while (opcao != 0);
}