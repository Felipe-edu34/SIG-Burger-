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



void exibir_cardapio_relatorio() {


    FILE *arq_item;
    Itemcardapio item;
    char categoria_atual[50] = "";
    int encontrou = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                          CARDÁPIO                        ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");

    arq_item = fopen("item_cardapio.dat", "rb");
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



void relatorio_cardapio_itens_disponiveis() {

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           ITEM DISPONIVEIS NO CARDAPIO           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    FILE* arq_cardapio = fopen("item_cardapio.dat","rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }
    while (fread(item, sizeof(Cliente), 1, arq_cardapio) == 1) {
        if(item->disponivel == 1){
        exibir_item(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();
}


void exibir_itens_indisponiveis() {
   limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           ITEM INDISPONIVEIS NO CARDAPIO         ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    FILE* arq_cardapio = fopen("item_cardapio.dat","rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }
    while (fread(item, sizeof(Cliente), 1, arq_cardapio) == 1) {
        if(item->disponivel == 1){
        exibir_item(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();
}



void procurar_item_por_categoria(){

    char categoria_lida[15];
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          PROCURAR ITEM POR CATEGORIA             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    printf("digite a categoria dos item que voce quer ver: ");
    ler_string(categoria_lida, 15);


    FILE* arq_cardapio = fopen("item_cardapio.dat","rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }

    while (fread(item, sizeof(Cliente), 1, arq_cardapio) == 1) {
        if (strstr(item->categoria, categoria_lida) != NULL) {
            exibir_item(item);
        }
    }

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



void exibir_todo_o_estoque(){

    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          EXIBIR TODOS OS ITENS DO ESTOQUE        ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen("item_estoque.dat","rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }

    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        exibir_item_estoque(prod);
    }


    fclose(arq_estoque);
    free(prod);
    pausar();
}



void exibir_itens_com_baixa_quantidade() {
    
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║        ITENS COM BAIXA QUANTIDADE NO ESTOQUE     ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen("item_estoque.dat","rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }

    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if(prod->quantidade < 5){
        exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
}



void exibir_itens_indisponiveis_estoque() {
    
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║        ITENS INDISPONIVEIS NO ESTOQUE            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen("item_estoque.dat","rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }

    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if(prod->ativo == 0){
        exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
}



void exibir_itens_por_categoria() {
    char categoria_lida[30];
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          PROCURAR ITEM POR CATEGORIA             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    printf("digite a categoria dos item que voce quer ver: ");
    ler_string(categoria_lida, 30);

    FILE* arq_estoque = fopen("item_estoque.dat","rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }
    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if (strstr(prod->categoria, categoria_lida) != NULL) {
            exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
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
    int opcao, opcao_estoque, opcao_cardapio;
    

    do {
        menu_relatorio();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                do {
                    relatorio_cardapio();
                    scanf("%d", &opcao_cardapio);
                    limparBuffer();

                    switch (opcao_cardapio) {
                        case 1:
                            exibir_cardapio_relatorio();
                            break;
                        case 2:
                            relatorio_cardapio_itens_disponiveis();
                            break;
                        case 3:
                            exibir_itens_indisponiveis();
                            break;
                        case 4:
                            procurar_item_por_categoria();
                            break;
                        case 0:
                            printf("Voltando ao Menu de Relatórios...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao_cardapio != 0);
                break;

            case 2:
                do {
                    relatorio_estoque();
                    scanf("%d", &opcao_estoque);
                    limparBuffer();

                    switch (opcao_estoque) {
                        case 1:
                            exibir_todo_o_estoque();
                            break;
                        case 2:
                            exibir_itens_com_baixa_quantidade();
                            break;
                        case 3:
                            exibir_itens_indisponiveis();
                            break;
                        case 4:
                            exibir_itens_por_categoria();
                            break;
                        case 0:
                            printf("Voltando ao Menu de Relatórios...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao_estoque != 0);
                break;
            case 0:
                printf("Voltando ao Menu Principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        pausar();
    } while (opcao != 0);
}