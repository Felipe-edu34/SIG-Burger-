#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"

#define ARQUIVO_ESTOQUE "estoque.dat"

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
    exibir_item(prod);
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
    printf("║ quantidade:   %i\n", prod->quantidade);
    printf("║ validade:       %s\n", prod->validade);
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


void cadastrar_item(){

    Produto* prod;
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           CADASTRAR PRODUTO AO ESTOQUE           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Nome do Produto: ");
    ler_string(prod->nome, sizeof(prod->nome));

    printf("► Categoria: ");
    ler_string(prod->categoria, sizeof(prod->categoria));

    printf("► Quantidade inicial: ");
    if (scanf("%d", &prod->quantidade) != 1 || prod->quantidade < 0) {
        limparBuffer();
        printf("\nQuantidade inválida.\n");
        pausar();
        return;
    }
    limparBuffer();

    printf("► Validade (dd/mm/aaaa): ");
    ler_string(prod->validade, sizeof(prod->validade));

    prod->ativo = 1;

    if (!confirma_dados_produto(prod)) {
        printf("\nCadastro cancelado pelo usuário.\n");
        pausar();
        return;
    }

    gravar_produto(prod);
    printf("\n Produto cadastrado com sucesso!\n");
    pausar();
}



void remover_produto() {
    FILE *arq;
    Produto *prod = (Produto*) malloc(sizeof(Produto));
    int numero, contador = 0;
    long pos_arquivo;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 REMOVER PRODUTO DO ESTOQUE        ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_ESTOQUE, "rb");
    if (arq == NULL) {
        printf("Nenhum produto cadastrado ainda.\n");
        free(prod);
        pausar();
        return;
    }

    // Exibir produtos ativos
    printf("Produtos disponíveis:\n\n");
    while (fread(prod, sizeof(Produto), 1, arq) == 1) {
        if (prod->ativo == 1) {
            contador++;
            printf(" %d - %s  (Qtd: %d, Validade: %s)\n",
                   contador, prod->nome, prod->quantidade, prod->validade);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum produto ativo encontrado.\n");
        free(prod);
        pausar();
        return;
    }

    printf("\nDigite o número do produto que deseja remover: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        free(prod);
        pausar();
        return;
    }

    // Reabrir arquivo para edição
    arq = fopen(ARQUIVO_ESTOQUE, "r+b");
    contador = 0;

    while (fread(prod, sizeof(Produto), 1, arq) == 1) {
        if (prod->ativo == 1) {
            contador++;
            if (contador == numero) {
                pos_arquivo = ftell(arq) - sizeof(Produto);
                break;
            }
        }
    }

    printf("\nConfirmar remoção do produto '%s'? (S/N): ", prod->nome);
    char resp;
    scanf(" %c", &resp);
    limparBuffer();

    if (resp == 'S' || resp == 's') {
        prod->ativo = 0;
        fseek(arq, pos_arquivo, SEEK_SET);
        fwrite(prod, sizeof(Produto), 1, arq);
        printf("\n✅ Produto removido com sucesso!\n");
    } else {
        printf("\nRemoção cancelada.\n");
    }

    fclose(arq);
    free(prod);
    pausar();
}

