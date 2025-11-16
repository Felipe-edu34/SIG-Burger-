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