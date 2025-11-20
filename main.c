#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cardapio.h"
#include "cliente.h"
#include "utils.h"
#include "estoque.h"
#include "relatorio.h"
#include "financeiro.h"



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



void sobre_o_sistema() {

    limpar_tela();
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║                    SOBRE O SIG-BURGER             ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║                                                   ║\n");
    printf("║   O SIG-Burger é um sistema desenvolvido para     ║\n");
    printf("║   facilitar o gerenciamento de pedidos, itens     ║\n");
    printf("║   de cardápio e operações internas de uma         ║\n");
    printf("║   hamburgueria. Criado com foco em eficiência,    ║\n");
    printf("║   simplicidade e organização, o projeto faz parte ║\n");
    printf("║   das atividades acadêmicas dos desenvolvedores.  ║\n");
    printf("║                                                   ║\n");
    printf("║   Desenvolvedores:                                ║\n");
    printf("║      • Felipe Eduardo                             ║\n");
    printf("║      • Elisson Tavares                            ║\n");
    printf("║                                                   ║\n");
    printf("║   Contato:                                        ║\n");
    printf("║      • felipe.fernandes.710@ufrn.edu.br           ║\n");
    printf("║      • elisson.tavares.dev@gmail.com              ║\n");
    printf("║                                                   ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    pausar();
}



int main(void) {
    int opcao;
    tela_inicial();
    sleep(3);

    do {
        tela_menu_principal();
        scanf("%d", &opcao);
        limparBuffer();


        switch (opcao) {
            case 1:
                cliente();
                break;
            case 2:
                printf("Módulo Pedidos selecionado.\n");
                // Chamar função do módulo Pedidos
                break;
            case 3:
                cardapio();
                break;
            case 4:
                estoque();
                break;
            case 5:
                financeiro();
                break;
            case 6:
                relatorio();
                break;
            case 7:
                sobre_o_sistema();
                break;

            case 0:
                limpar_tela();
                printf("╔══════════════════════════════════════════════════════════════╗\n");
                printf("║                                                              ║\n");
                printf("║          Obrigado por utilizar o sistema SIG-BURGER!         ║\n");
                printf("║                                                              ║\n");
                printf("║   Desenvolvido com dedicação e fome por conhecimento.        ║\n");
                printf("║   Volte sempre — a casa é sua e o código também!             ║\n");
                printf("║                                                              ║\n");
                printf("╚══════════════════════════════════════════════════════════════╝\n");
                return 0;
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                pausar();

        }

    } while (opcao != 0);

}
