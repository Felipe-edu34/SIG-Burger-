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

int gerar_numero_pedido() {
    FILE *arq;
    Pedido ped;
    int maior_numero = 0;

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        return 1;
    }

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.numero_pedido > maior_numero) {
            maior_numero = ped.numero_pedido;
        }
    }
    fclose(arq);

    return maior_numero + 1;
}
