#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cardapio.h"
#include "leitura.h"

#define ARQUIVO_ITEM "item_cardapio.dat"


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



ResultadoBuscacardapio selecionar_produto_cardapio() {
    FILE *arq;
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    int contador = 0, numero;
    ResultadoBuscacardapio resultado = {0, NULL, 0};

    arq = fopen(ARQUIVO_ITEM, "rb");
    if (arq == NULL) {
        printf("Nenhum produto cadastrado.\n");
        free(item);
        return resultado;
    }

    printf("Produtos disponíveis:\n\n");
    while (fread(item, sizeof(Itemcardapio), 1, arq) == 1) {
        if (item->disponivel == 1) {
            contador++;
            printf(" %d - %s ( Categoria: %s)\n",
                   contador, item->nome, item->categoria);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum produto ativo.\n");
        free(item);
        return resultado;
    }

    printf("\nEscolha o produto: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        free(item);
        return resultado;
    }

    // Reabrir arquivo para obter a posição real
    arq = fopen(ARQUIVO_ITEM, "rb");
    contador = 0;

    while (fread(item, sizeof(Itemcardapio), 1, arq) == 1) {

        if (item->disponivel == 1) {
            contador++;
            if (contador == numero) {

                resultado.pos = ftell(arq) - sizeof(Itemcardapio);
                resultado.item = item;
                resultado.existe = 1;

                fclose(arq);
                return resultado;
            }
        }
    }

    fclose(arq);
    free(item);
    return resultado;
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


    FILE *arq_item = fopen(ARQUIVO_ITEM, "ab");              // Abre o arquivo em modo anexar (append)
    if (arq_item == NULL) {
        return;
    }

    fwrite(item, sizeof(Itemcardapio), 1, arq_item);
    fclose(arq_item);
}



void cadastrar_item_ao_cardapio() {

    limpar_tela();
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              CADASTRAR ITEM AO CARDÁPIO          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    
     printf("► Nome do Item: ");
    ler_string(item->nome, sizeof(item->nome));

    printf("► Categoria: ");
    ler_string(item->categoria, sizeof(item->categoria));

    printf("► Descrição: ");
    ler_string(item->descricao, sizeof(item->descricao));

    printf("► Preço (R$): ");
    if (scanf("%f", &item->preco) != 1) {
        limparBuffer();
        printf("Preço inválido.\n");
        pausar();
        return;
    }
    limparBuffer();

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
    
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              EXCLUIR ITEM DO CARDÁPIO            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    ResultadoBuscacardapio r = selecionar_produto_cardapio();

    if (!r.existe) { pausar(); return; }

    printf("\nConfirmar remoção de '%s'? (S/N): ", r.item->nome);
    char resp;
    scanf(" %c", &resp);
    limparBuffer();

    if (resp != 'S' && resp != 's') {
        printf("\nRemoção cancelada.\n");
        free(r.item);
        pausar();
        return;
    }

    FILE *arq = fopen(ARQUIVO_ITEM, "r+b");
    fseek(arq, r.pos, SEEK_SET);

    r.item->disponivel = 0;

    fwrite(r.item, sizeof(Itemcardapio), 1, arq);

    fclose(arq);
    free(r.item);

    printf("\nItem removido com sucesso!\n");
    pausar();
}



void editar_item_do_cardapio() {
    

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              EDITAR ITEM DO CARDÁPIO             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    ResultadoBuscacardapio r = selecionar_produto_cardapio();
    if (!r.existe) { pausar(); return; }
    limpar_tela();
    
    ler_nome_produto(r.item->nome);
    ler_categoria_item(r.item->categoria);
    ler_descricao_item(r.item->descricao);
    ler_preco_item(&r.item->preco);
    FILE *arq = fopen(ARQUIVO_ITEM, "r+b");
    fseek(arq, r.pos, SEEK_SET);
    fwrite(r.item, sizeof(Itemcardapio), 1, arq);
    fclose(arq);
    free(r.item);

    printf("\nItem atualizado!\n");
    pausar();


}



void pesquisar_item_do_cardapio() {
    FILE* arq_item;
    Itemcardapio item;
    int numero, contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             PESQUISAR ITEM DO CARDÁPIO           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    arq_item = fopen(ARQUIVO_ITEM, "rb");
    if (arq_item == NULL) {
        printf("Nenhum item cadastrado ainda.\n");
        pausar();
        return;
    }

    printf("Itens cadastrados:\n\n");
    while (fread(&item, sizeof(Itemcardapio), 1, arq_item) == 1) {
        if (item.disponivel == 1) {
            contador++;
            printf(" %d - %s  (R$ %.2f)\n", contador, item.nome, item.preco);
        }
    }
    fclose(arq_item);

    if (contador == 0) {
        printf("\nNenhum item ativo encontrado.\n");
        pausar();
        return;
    }

    printf("\nDigite o número do item que deseja visualizar: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq_item = fopen(ARQUIVO_ITEM, "rb");
    contador = 0;
    while (fread(&item, sizeof(Itemcardapio), 1, arq_item) == 1) {
        if (item.disponivel == 1) {
            contador++;
            if (contador == numero) {
                limpar_tela();
                exibir_item(&item);
                fclose(arq_item);
                pausar();
                return;
            }
        }
    }

    fclose(arq_item);
    printf("\nItem não encontrado.\n");
    pausar();
}



void exibir_cardapio() {
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

  

void cardapio() {
    int opcao;

    do {
        menu_cardapio();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrar_item_ao_cardapio();
                break;
            case 2:
                excluir_item_do_cardapio();
                break;
            case 3:
                editar_item_do_cardapio();
                break;
            case 4:
                pesquisar_item_do_cardapio();
                break;
            case 5:
                exibir_cardapio();
                break;
            case 0:
                printf("Voltando ao Menu Principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                pausar();
        }

    } while (opcao != 0);
}