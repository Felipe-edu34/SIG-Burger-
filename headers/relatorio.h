#ifndef RELATORIO_H
#define RELATORIO_H
#include "estoque.h"
#include "cardapio.h"



void menu_relatorio();
void relatorio();

//////////////////////////////RELATÓRIOS DE CARDÁPIO////////////////////////////////

void relatorio_cardapio();
void listar_itens_por_preco();
void exibindo_cardapio_por_categoria();
void relatorio_itens_por_status(int status, const char* titulo);
void relatorio_cardapio_itens_disponiveis();
void relatorio_cardapio_itens_indisponiveis();
void procurar_item_por_categoria();
void liberar_lista_cardapio(NodeItem *lista);
void exibir_lista_itens(NodeItem *lista);
NodeItem* montar_lista_itens_ordenados_preco();

///////////////////////////////RELATÓRIOS DE ESTOQUE////////////////////////////////

void relatorio_estoque();
void exibir_itens_com_baixa_quantidade();
void exibir_itens_indisponiveis_estoque();
void exibir_itens_por_nome();
void exibindo_item_do_cardapio_por_preco();
void listar_estoque_por_quantidade();
void liberar_lista(Nodeproduto *lista);
void ordenar_por_quantidade(Nodeproduto *lista);
Nodeproduto* montar_lista_estoque();
void listar_todo_estoque();
void exibir_lista_estoque(Nodeproduto *lista);
void listar_itens_por_preco();

//////////////////////////////RELATÓRIOS DE CLIENTES////////////////////////////////

void relatorio_clientes();
void relatorio_clientes_com_ultimo_pedido();

//////////////////////////////RELATÓRIOS DE PEDIDOS////////////////////////////////

void exibir_pedidos_por_status();
void relatorio_delivery_vs_local();
void relatorio_itens_mais_pedidos();
void relatorio_historico_cliente();
void relatorio_pedidos_por_data();
void relatorio_pedidos();




#endif // RELATORIO_H