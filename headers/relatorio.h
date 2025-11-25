#ifndef RELATORIO_H
#define RELATORIO_H
#include "estoque.h"
#include "cardapio.h"



void menu_relatorio();
void relatorio();
void relatorio_estoque();
void relatorio_cardapio();
void exibindo_cardapio_por_categoria();
void relatorio_cardapio_itens_disponiveis();
void relatorio_cardapio_itens_indisponiveis();
void procurar_item_por_categoria();
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
NodeItem* montar_lista_itens_ordenados_preco();
void liberar_lista_cardapio(NodeItem *lista);
void listar_itens_por_preco();
void exibir_lista_itens(NodeItem *lista);
void relatorio_clientes();
void relatorio_clientes_com_ultimo_pedido();



#endif // RELATORIO_H