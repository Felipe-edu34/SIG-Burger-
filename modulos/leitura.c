#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "cardapio.h"


void ler_categoria_cardapio(char *categoria) {
    do {
    printf("====================================\n");
    printf("|  INSIRA A CATEGORIA': ");
    ler_string(categoria, 20);
    } while (!valida_categoria(categoria));
}



void ler_nome_produto(char *nome) {
    do {
    printf("====================================\n");
    printf("|  INSIRA O NOME DO PRODUTO: ");
    ler_string(nome, 50);
    } while (!valida_nome(nome));
}