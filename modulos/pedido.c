#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pedido.h"
#include "cardapio.h"

#define ARQUIVO_PEDIDOS "dados/pedidos.dat"

void menu_pedidos() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 MÓDULO PEDIDOS                   ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Cadastrar Pedido                            ║\n");
    printf("║ ► 2. Listar Pedidos                              ║\n");
    printf("║ ► 3. Editar Pedido                               ║\n");
    printf("║ ► 4. Pesquisar Pedido                            ║\n");
    printf("║ ► 5. Excluir Pedido                              ║\n");
    printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}
