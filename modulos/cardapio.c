#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cardapio.h"


void menu_cardapio() {

    limpar_tela();
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



int confirma_dados_cardapio(Itemcardapio* item) {
    char confirm;
    limpar_tela();
    exibir_item(item);
    printf("Os dados do item novo do cardapio estão corretos? (S/N): ");
    scanf(" %c", &confirm);
    limparBuffer();

    if (confirm == 'S' || confirm == 's') {
        return 1;
    } else {
        return 0;
    }
}



void exibir_item(Itemcardapio* item){

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             ITEM CADASTRADO (VISUALIZAÇÃO)       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Nome:        %s\n", item->nome);
    printf("║ Categoria:   %s\n", item->categoria);
    printf("║ Descrição:   %s\n", item->descricao);
    printf("║ Preço:       R$ %.2f\n", item->preco);
    printf("╚══════════════════════════════════════════════════╝\n");
}



void gravar_item(Itemcardapio* item){


    FILE *arq_item_cardapio = fopen("item_cardapio.dat", "ab");              // Abre o arquivo em modo anexar (append)
    if (arq_item_cardapio == NULL) {
        return;
    }

    fwrite(item, sizeof(Itemcardapio), 1, arq_item_cardapio);
    fclose(arq_item_cardapio);
}



void cadastrar_item_ao_cardapio() {

    limpar_tela();
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              CADASTRAR ITEM AO CARDÁPIO          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    printf("► Nome do Item: ");
    ler_string(item->nome, 50);

    printf("► Categoria: ");
    ler_string(item->categoria, 30);

    printf("► Descrição: ");
    ler_string(item->descricao, 100);

    printf("► Preço (R$): ");
    scanf("%f", &item->preco);

    item->disponivel = 1;

    if(!confirma_dados_cardapio(item)) {
        printf("\nCadastro cancelado pelo usuário.\n");
        free(item);
        pausar();
        return;
    }
    gravar_item(item);
    printf("\n Item cadastrado com sucesso!\n");
    free(item);
    pausar();
}



void excluir_item_do_cardapio() {

    Itemcardapio* item;
    int encontrado = 0;
    FILE *arq_item_cardapio;
    char confirm;
    item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    if (item == NULL) {
        printf("Erro ao alocar memoria para o cardapio.\n");
        return;
    }

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              EXCLUIR ITEM DO CARDAPIO            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

  
}



void cardapio() {
    int opcao;

    do {
        menu_cardapio();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_item_ao_cardapio();
                break;
            case 2:
                excluir_item_do_cardapio();
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