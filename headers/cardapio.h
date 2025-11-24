#ifndef CARDAPIO_H
#define CARDAPIO_H

typedef struct Itemcardapio Itemcardapio;


struct Itemcardapio {
    char nome[50];
    char categoria[20];
    char descricao[100];
    float preco;
    int disponivel;
};



typedef struct {
    long pos;        // posição no arquivo
    Itemcardapio *item;   // ponteiro para o produto encontrado
    int existe;      // 1 = encontrado, 0 = não
} ResultadoBuscacardapio;

ResultadoBuscacardapio selecionar_produto_cardapio();

typedef struct NodeItem {
    Itemcardapio dado;
    struct NodeItem* prox;
} NodeItem;

void tela_menu_principal();
void cardapio();
int confirma_dados_cardapio(Itemcardapio* item);
void exibir_item(Itemcardapio* item);
void gravar_item(Itemcardapio* item);
void cadastrar_item_ao_cardapio();
void excluir_item_do_cardapio();

#endif // CARDAPIO_H