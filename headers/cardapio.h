#ifndef CARDAPIO_H
#define CARDAPIO_H

typedef struct {
    int codigo;
    char nome[50];
    char categoria[20];
    char descricao[100];
    float preco;
    int disponivel;
} Itemcardapio;

void tela_menu_principal();
void cardapio();
int confirma_dados_cardapio(Itemcardapio* item);
void exibir_item(Itemcardapio* item);
void gravar_item(Itemcardapio* item);
int gerar_codigo();
void cadastrar_item_ao_cardapio();

#endif // CARDAPIO_H