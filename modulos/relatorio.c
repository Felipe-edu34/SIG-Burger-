#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "relatorio.h"
#include "cliente.h"
#include "cardapio.h"

#define ARQUIVO_ITEM "item_cardapio.dat"

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



void exibir_cardapio_relatorio() {


    FILE *arq_item;
    Itemcardapio item;
    char categoria_atual[50] = "";
    int encontrou = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                          CARDÁPIO                        ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");

    arq_item = fopen(ARQUIVO_ITEM, "rb");
    if (arq_item == NULL) {
        printf("║ Nenhum item cadastrado ainda.                           ║\n");
        printf("╚══════════════════════════════════════════════════════════╝\n");
        pausar();
        return;
    }

    // Ler arquivo uma vez, em ordem
    while (fread(&item, sizeof(Itemcardapio), 1, arq_item) == 1) {
        if (item.disponivel == 0)
            continue;

        // Quando muda a categoria, imprime título
        if (strcmp(categoria_atual, item.categoria) != 0) {
            if (encontrou)
                printf("╠══════════════════════════════════════════════════════════╣\n");

            strcpy(categoria_atual, item.categoria);
            printf("║   %-55s║\n", categoria_atual);
            printf("║ -------------------------------------------------------- ║\n");
        }

        encontrou = 1;

        // Exibe item formatado
        char linha[70];
        snprintf(linha, sizeof(linha), "• %-28s R$ %6.2f", item.nome, item.preco);
        printf("║ %-59s║\n", linha);

        if (strlen(item.descricao) > 0) {
            char desc[110];
            snprintf(desc, sizeof(desc), "↳ %s", item.descricao);
            printf("║    %-56s║\n", desc);
        }

        printf("║                                                          ║\n");
    }

    if (!encontrou) {
        printf("║ Nenhum item ativo encontrado.                           ║\n");
    }

    printf("╚══════════════════════════════════════════════════════════╝\n");
    fclose(arq_item);
    pausar();
}



void relatorio_cardapio() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATÓRIO D0 CARDÁPIO              ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. exibir cardapio                             ║\n");
    printf("║ ► 2. Itens disponíveis                           ║\n");
    printf("║ ► 3. Itens indisponíveis                         ║\n");
    printf("║ ► 4. procurar item por categoria                 ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
}



void relatorio_estoque() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATÓRIO DO ESTOQUE               ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. exibir estoque completo                     ║\n");
    printf("║ ► 2. Itens com baixa quantidade                  ║\n");
    printf("║ ► 3. Itens indisponíveis                         ║\n");
    printf("║ ► 4. Itens por categoria                         ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
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