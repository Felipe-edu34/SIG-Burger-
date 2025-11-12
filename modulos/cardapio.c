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
    printf("║ Código:      %d\n", item->codigo);
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



int gerar_codigo() {
    FILE *arq;
    int codigo = 1;

    // tenta abrir o arquivo em modo leitura binária
    arq = fopen("codigo.dat", "rb");

    // se já existir, lê o último código
    if (arq != NULL) {
        fread(&codigo, sizeof(int), 1, arq);
        fclose(arq);
        codigo++; // incrementa para o próximo
    }

    // salva o novo código atualizado
    arq = fopen("codigo.dat", "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de código.\n");
        return -1;
    }

    fwrite(&codigo, sizeof(int), 1, arq);
    fclose(arq);

    return codigo;
}



void cadastrar_item_ao_cardapio() {

    limpar_tela();
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              CADASTRAR ITEM AO CARDÁPIO          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    item->codigo = gerar_codigo();
    limparBuffer();

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