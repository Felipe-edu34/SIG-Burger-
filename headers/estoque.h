#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct {         
    char nome[50];            
    char categoria[30];       
    int quantidade;                      
    char validade[11];        
    int ativo;                
} Produto;

typedef struct Nodeproduto {
    Produto dado;
    struct Nodeproduto* prox;
} Nodeproduto;

typedef struct {
    long pos;        // posição no arquivo
    Produto *prod;   // ponteiro para o produto encontrado
    int existe;      // 1 = encontrado, 0 = não
} ResultadoBuscaEstoque;

void menu_estoque();
void estoque();
void pesquisar_produto();
void editar_produto();
void remover_produto();
void cadastrar_produto();
void gravar_item_estoque(Produto* prod);
void exibir_item_estoque(Produto* prod);
int confirma_dados_estoque(Produto* prod);
ResultadoBuscaEstoque selecionar_produto_estoque();



#endif 