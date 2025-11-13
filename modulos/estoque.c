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



void cadastrar_item(){
    void cadastrar_produto() {
    limpar_tela();
    Produto* prod;

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           CADASTRAR PRODUTO AO ESTOQUE           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Nome do Produto: ");
    ler_string(prod->nome, sizeof(prod->nome));

    printf("► Categoria: ");
    ler_string(prod->categoria, sizeof(prod->categoria));

    printf("► Quantidade inicial: ");
    if (scanf("%d", &prod->quantidade) != 1 || produto->quantidade < 0) {
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
    printf("\n✅ Produto cadastrado com sucesso!\n");
    pausar();
}

}
