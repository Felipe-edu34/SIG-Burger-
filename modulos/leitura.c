#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "cardapio.h"
#include "leitura.h"
#include "valida.h"


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
    } while (!validar_nome_produto(nome));
}



void ler_nome_item_cardapio(char *nome) {
    do {
    printf("► Nome do Item: ");
    ler_string(nome, 50);
    } while (!validar_nome_produto(nome));
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



float ler_preco(float *preco) {
    do {
    printf("► Preço (R$): ");
    scanf("%f", preco);
    limparBuffer();
    } while (!valida_preco(preco));
    return *preco;
}



void ler_quantidade(int *quantidade) {
    printf("► Quantidade: ");
    scanf("%d", quantidade);
    limparBuffer();
}




void ler_nome_cliente(char *nome) {
    do {
        printf("► Nome do Cliente: ");
        ler_string(nome, 100);
        if (!valida_nome(nome)) {
            printf("Nome inválido! Mínimo 3 caracteres, apenas letras.\n");
        }
    } while (!valida_nome(nome));
}

void ler_telefone_cliente(char *telefone) {
    do {
        printf("► Telefone (apenas números): ");
        ler_string(telefone, 15);
        if (!valida_telefone(telefone)) {
            printf("Telefone inválido! Digite 10 ou 11 dígitos numéricos.\n");
        }
    } while (!valida_telefone(telefone));
}

void ler_endereco_entrega(char *endereco) {
    printf("► Endereço de Entrega (deixe em branco para consumo no local): ");
    ler_string(endereco, 200);
    
    if (strlen(endereco) > 0) {
        while (!valida_endereco(endereco)) {
            printf("Endereço inválido! Mínimo 10 caracteres para delivery.\n");
            printf("► Endereço de Entrega: ");
            ler_string(endereco, 200);
        }
    }
}

void ler_data_pedido(char *data) {
    do {
        printf("► Data (dd/mm/aaaa): ");
        ler_string(data, 11);
        if (!valida_data(data)) {
            printf("Data inválida! Use o formato dd/mm/aaaa.\n");
        }
    } while (!valida_data(data));
}
