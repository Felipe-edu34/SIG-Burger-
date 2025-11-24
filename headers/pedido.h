#ifndef PEDIDO_H
#define PEDIDO_H

#include "cardapio.h"

#define MAX_ITENS_PEDIDO 20
#define TAXA_ENTREGA 3.00

typedef struct {
    Itemcardapio item;
    int quantidade;
} ItemPedido;

typedef struct {
    int numero_pedido;
    char nome_cliente[100];
    char telefone_cliente[15];
    char endereco_entrega[200];
    ItemPedido itens[MAX_ITENS_PEDIDO];
    int total_itens;
    float valor_total;
    float taxa_entrega;
    int eh_delivery;
    char status[20];
    char data[11];
    int ativo;
} Pedido;

void menu_pedidos();
void pedido();
void cadastrar_pedido();
void listar_pedidos();
void editar_pedido();
void excluir_pedido();
void pesquisar_pedido();
void exibir_pedido(Pedido* ped);
int confirma_dados_pedido(Pedido* ped);
void gravar_pedido(Pedido* ped);
int gerar_numero_pedido();

#endif