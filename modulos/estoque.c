#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "leitura.h"

#define ARQUIVO_ESTOQUE "dados/estoque.dat"

void menu_estoque(){
    limpar_tela();
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║               MÓDULO DE ESTOQUE                  ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║                                                  ║\n");
        printf("║ ► 1. Cadastrar Produto                           ║\n");
        printf("║ ► 2. Editar Produto                              ║\n");
        printf("║ ► 3. Pesquisar Produto                           ║\n");
        printf("║ ► 4. Excluir Produto                             ║\n");
        printf("║                                                  ║\n");
        printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
        printf("║                                                  ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        printf("Escolha uma opção: ");
}



int confirma_dados_estoque(Produto* prod) {
    char confirm;
    limpar_tela();
    exibir_item_estoque(prod);
    printf("Os dados do item novo do estoque estão corretos? (S/N): ");
    scanf(" %c", &confirm);
    limparBuffer();

    if (confirm == 'S' || confirm == 's') {
        return 1;
    } else {
        return 0;
    }
}



void exibir_item_estoque(Produto* prod){

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             ITEM CADASTRADO (VISUALIZAÇÃO)       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Nome:        %s\n", prod->nome);
    printf("║ Categoria:   %s\n", prod->categoria);
    printf("║ Quantidade:  %i\n", prod->quantidade);
    printf("║ Validade:    %s\n", prod->validade);
    printf("╚══════════════════════════════════════════════════╝\n");
}



void gravar_item_estoque(Produto* prod){


    FILE *arq_item = fopen(ARQUIVO_ESTOQUE, "ab");              // Abre o arquivo em modo anexar (append)
    if (arq_item == NULL) {
        return;
    }

    fwrite(prod, sizeof(Produto), 1, arq_item);
    fclose(arq_item);
}



void cadastrar_produto(){

    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           CADASTRAR PRODUTO AO ESTOQUE           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    ler_nome_produto(prod->nome);
    ler_categoria_estoque(prod->categoria);
    ler_quantidade(&prod->quantidade);
    ler_validade(prod->validade);
    prod->ativo = 1;

    if (!confirma_dados_estoque(prod)) {
        printf("\nCadastro cancelado pelo usuário.\n");
        pausar();
        return;
    }

    gravar_item_estoque(prod);
    printf("\n Produto cadastrado com sucesso!\n");
    pausar();
}



void remover_produto() {

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              REMOVER PRODUTO DO ESTOQUE          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    ResultadoBuscaEstoque r = selecionar_produto_estoque();

    if (!r.existe) { pausar(); return; }

    printf("\nConfirmar remoção de '%s'? (S/N): ", r.prod->nome);
    char resp;
    scanf(" %c", &resp);
    limparBuffer();

    if (resp != 'S' && resp != 's') {
        printf("\nRemoção cancelada.\n");
        free(r.prod);
        pausar();
        return;
    }

    FILE *arq = fopen(ARQUIVO_ESTOQUE, "r+b");
    fseek(arq, r.pos, SEEK_SET);

    r.prod->ativo = 0;

    fwrite(r.prod, sizeof(Produto), 1, arq);

    fclose(arq);
    free(r.prod);

    printf("\nProduto removido com sucesso!\n");
    pausar();
}



void editar_produto() {
    
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             EDITAR PRODUTO DO ESTOQUE            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    
    ResultadoBuscaEstoque r = selecionar_produto_estoque();
    if (!r.existe) { pausar(); return; }

    printf("\n► Editando produto: %s\n", r.prod->nome);

    ler_nome_produto(r.prod->nome);
    ler_categoria_estoque(r.prod->categoria);
    ler_quantidade(&r.prod->quantidade);
    ler_validade(r.prod->validade);

    FILE *arq = fopen(ARQUIVO_ESTOQUE, "r+b");
    fseek(arq, r.pos, SEEK_SET);
    fwrite(r.prod, sizeof(Produto), 1, arq);
    fclose(arq);

    free(r.prod);

    printf("\nProduto atualizado!\n");
    pausar();
}



void pesquisar_produto() {

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 PESQUISAR PRODUTO                ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    ResultadoBuscaEstoque r = selecionar_produto_estoque();
    if (!r.existe) { pausar(); return; }

    limpar_tela();
    printf("\n► Detalhes do produto:\n");
    exibir_item_estoque(r.prod);

    free(r.prod);
    pausar();
    
}



ResultadoBuscaEstoque selecionar_produto_estoque() {
    FILE *arq;
    Produto *prod = (Produto*) malloc(sizeof(Produto));
    int contador = 0, numero;
    ResultadoBuscaEstoque resultado = {0, NULL, 0};

    arq = fopen(ARQUIVO_ESTOQUE, "rb");
    if (arq == NULL) {
        printf("Nenhum produto cadastrado.\n");
        free(prod);
        return resultado;
    }

    printf("Produtos disponíveis:\n\n");
    while (fread(prod, sizeof(Produto), 1, arq) == 1) {
        if (prod->ativo == 1) {
            contador++;
            printf(" %d - %s (Qtd: %d, Validade: %s)\n",
                   contador, prod->nome, prod->quantidade, prod->validade);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum produto ativo.\n");
        free(prod);
        return resultado;
    }

    printf("\nEscolha o produto: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        free(prod);
        return resultado;
    }

    // Reabrir arquivo para obter a posição real
    arq = fopen(ARQUIVO_ESTOQUE, "rb");
    contador = 0;

    while (fread(prod, sizeof(Produto), 1, arq) == 1) {

        if (prod->ativo == 1) {
            contador++;
            if (contador == numero) {

                resultado.pos = ftell(arq) - sizeof(Produto);
                resultado.prod = prod;
                resultado.existe = 1;

                fclose(arq);
                return resultado;
            }
        }
    }

    fclose(arq);
    free(prod);
    return resultado;
}



void estoque() {
    int opcao;

    do {
        menu_estoque();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrar_produto();
                break;
            case 2:
                editar_produto();
                break;
            case 3:
                pesquisar_produto();
                break;
            case 4:
                remover_produto();
                break;
            case 0:
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                pausar();
                break;
        }

    } while (opcao != 0);
}
