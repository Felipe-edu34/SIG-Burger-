#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cardapio.h"
#include "leitura.h"
#include "relatorio.h"

#define ARQUIVO_ITEM "dados/item_cardapio.dat"


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



int compararCategorias(const void *a, const void *b) {
    const Itemcardapio *itemA = (const Itemcardapio*)a;
    const Itemcardapio *itemB = (const Itemcardapio*)b;

    return strcmp(itemA->categoria, itemB->categoria);
}



ResultadoBuscacardapio selecionar_produto_cardapio() {
    FILE *arq;
    Itemcardapio temp;
    Itemcardapio *lista = NULL;
    int total = 0;

    ResultadoBuscacardapio resultado = {0, NULL, 0};

    arq = fopen(ARQUIVO_ITEM, "rb");
    if (!arq) {
        printf("Nenhum produto cadastrado.\n");
        return resultado;
    }

 
    while (fread(&temp, sizeof(Itemcardapio), 1, arq) == 1) {
        if (temp.disponivel == 1) {
            lista = realloc(lista, (total + 1) * sizeof(Itemcardapio));
            lista[total] = temp;
            total++;
        }
    }
    fclose(arq);

    if (total == 0) {
        printf("Nenhum produto ativo.\n");
        free(lista);
        return resultado;
    }

    qsort(lista, total, sizeof(Itemcardapio), compararCategorias);

    printf("Produtos disponíveis:\n\n");

    for (int i = 0; i < total; i++) {
        printf("%3d | %-20s | %-12s | %-80s | R$ %7.2f\n",
            i + 1,
            lista[i].nome,
            lista[i].categoria,
            lista[i].descricao,
            lista[i].preco
        );
    }


    int numero;
    printf("\nEscolha o produto: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > total) {
        printf("Número inválido!\n");
        free(lista);
        return resultado;
    }

    resultado.item = malloc(sizeof(Itemcardapio));
    *resultado.item = lista[numero - 1];
    resultado.existe = 1;

    // A posição no arquivo não faz mais sentido com ordenação,
    // então NÃO usamos mais ftell + offset.

    free(lista);
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

    
    ler_nome_item_cardapio(item->nome);
    ler_categoria_cardapio(item->categoria);
    ler_descricao_item_cardapio(item->descricao);
    ler_preco(&item->preco);

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
    
    ler_nome_item_cardapio(r.item->nome);
    ler_categoria_cardapio(r.item->categoria);
    ler_descricao_item_cardapio(r.item->descricao);
    ler_preco(&r.item->preco);
    FILE *arq = fopen(ARQUIVO_ITEM, "r+b");
    fseek(arq, r.pos, SEEK_SET);
    fwrite(r.item, sizeof(Itemcardapio), 1, arq);
    fclose(arq);
    free(r.item);

    printf("\nItem atualizado!\n");
    pausar();


}



void pesquisar_item_do_cardapio() {
    

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             PESQUISAR ITEM DO CARDÁPIO           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    ResultadoBuscacardapio r = selecionar_produto_cardapio();
    if (!r.existe) { pausar(); return; }

    limpar_tela();
    printf("\n► Detalhes do item do cardapio:\n");
    exibir_item(r.item);
    free(r.item);
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
                exibindo_cardapio_por_categoria();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                pausar();
        }

    } while (opcao != 0);
}