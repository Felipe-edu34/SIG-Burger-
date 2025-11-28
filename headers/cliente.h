
#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    char cpf[15];
    char nome[100];
    char telefone[15];
    char endereco[200];
    int status; // 1 = ativo, 0 = inativo
} Cliente;


void cliente(void);
void menu_clientes(void);
void cadastrar_cliente(void);
void listar_clientes(void);
void editar_cliente(void);
void excluir_cliente(void);

#endif