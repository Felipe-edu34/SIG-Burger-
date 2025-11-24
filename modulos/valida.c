#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "utils.h"
#include "cardapio.h"
#include "estoque.h"
#include "relatorio.h"


int valida_nome(const char* nome) {
    int i;
    int tamanho = strlen(nome);             //função para validar nome
    if (tamanho < 3) {
        return 0;
    }
    for (i = 0; i < tamanho; i++) {
        if (!((nome[i] >= 'A' && nome[i] <= 'Z') || (nome[i] >= 'a' && nome[i] <= 'z') || nome[i] == ' ')) {
            return 0;
        }
    }
    return 1;

}



int validar_nome_produto(const char *nome) {
    int i;
    int tamanho = strlen(nome);

    // mínimo de 3 caracteres
    if (tamanho < 3) {
        return 0;
    }

    for (i = 0; i < tamanho; i++) {
        // aceita letras, espaço e hífen '-'
        if (!(
              (nome[i] >= 'A' && nome[i] <= 'Z') ||
              (nome[i] >= 'a' && nome[i] <= 'z') ||
              nome[i] == ' ' ||
              nome[i] == '-'
             )) {
            return 0;
        }
    }
    return 1;
}



int valida_data(const char* data) {
    int dia, mes, ano;
    if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) != 3) {
        return 0;
    }
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return 0;
    }
    int dias_no_mes[] = {31, (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dia > dias_no_mes[mes - 1]) {
        return 0;
    }
    return 1;
}


int valida_categoria(const char* categoria) {
    int i;
    int tamanho = strlen(categoria);             //função para validar categoria
    if (tamanho < 3) {
        return 0;
    }
    for (i = 0; i < tamanho; i++) {
        if (!((categoria[i] >= 'A' && categoria[i] <= 'Z') || (categoria[i] >= 'a' && categoria[i] <= 'z') || categoria[i] == ' ')) {
            return 0;
        }
    }
    return 1;

}



int valida_descricao(const char* descricao) {
    int tamanho = strlen(descricao);             //função para validar descrição
    if (tamanho < 5) {
        return 0;
    }
    return 1;
}



int valida_preco(float* preco) {
    if (*preco < 0) {
        return 0;
    }

    return 1;
}



int valida_quantidade(int* quantidade) {
    if (*quantidade < 0) {
        return 0;
    }

    return 1;
}

int valida_telefone(const char* telefone) {
    int i;
    int tamanho = strlen(telefone);
    
    if (tamanho < 10 || tamanho > 11) {
        return 0;
    }
    
    for (i = 0; i < tamanho; i++) {
        if (!isdigit(telefone[i])) {
            return 0;
        }
    }
    
    return 1;
}

int valida_endereco(const char* endereco) {
    int tamanho = strlen(endereco);
    
    if (tamanho == 0) {
        return 1;
    }
    
    if (tamanho < 10) {
        return 0;
    }
    
    return 1;
}

