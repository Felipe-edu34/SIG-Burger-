#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct {         
    char nome[50];            
    char categoria[30];       
    int quantidade;                      
    char validade[11];        
    int ativo;                
} Produto;

void menu_estoque();

#endif 