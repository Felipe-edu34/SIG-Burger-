#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "cardapio.h"


void ler_categoria_cardapio(char *categoria) {
    do {
    printf("► Categoria: ");
    ler_string(categoria, 20);
    } while (!valida_categoria(categoria));
}



void ler_nome_produto(char *nome) {
    do {
    printf("► Nome do Produto: ");
    ler_string(nome, 50);
    } while (!valida_nome(nome));
}



void ler_nome_item_cardapio(char *nome) {
    do {
    printf("► Nome do Item: ");
    ler_string(nome, 50);
    } while (!valida_nome(nome));
}



void ler_categoria_estoque(char *categoria) {
    do {
    printf("► Categoria: ");
    ler_string(categoria, 30);
    } while (!valida_categoria(categoria));
}



void ler_descricao_item_cardapio(char *descricao) {
    do {
    printf("► Descrição: ");
    ler_string(descricao, 100);
    } while (!valida_descricao(descricao));
}



void ler_validade(char *validade) {
    do {
    printf("► Validade (dd/mm/aaaa): ");
    ler_string(validade, 11);
    } while (!valida_data(validade));
}



void ler_preco(float *preco) {
    do {
    printf("► Preço (R$): ");
    if (scanf("%f", preco) != 1) {
        limparBuffer();
        *preco = -1; // valor inválido
    }
    limparBuffer();
    } while (!valida_preco(*preco));
}







