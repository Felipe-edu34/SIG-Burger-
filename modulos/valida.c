#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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