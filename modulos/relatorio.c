#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "relatorio.h"
#include "cliente.h"
#include "cardapio.h"

#define ARQUIVO_ITEM "dados/item_cardapio.dat"
#define ARQUIVO_ESTOQUE "dados/estoque.dat"


void menu_relatorio(){
    limpar_tela();
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║               MÓDULO DE RELATÓRIOS               ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║                                                  ║\n");
        printf("║ ► 1. Relatório de cardapio                       ║\n");
        printf("║ ► 2. Relatório de Estoque                        ║\n");
        printf("║ ► 3. Relatório de Clientes                       ║\n");
        printf("║ ► 4. Relatório de pedidos                        ║\n");
        printf("║ ► 5. Relatorio financeiro                        ║\n");
        printf("║                                                  ║\n");
        printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
        printf("║                                                  ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        printf("Escolha uma opção: ");
}



void exibindo_cardapio_por_categoria() {
    FILE *arq_item;
    Itemcardapio temp;

    NodeItem *lista = NULL;
    NodeItem *novo, *atual, *anter;

    arq_item = fopen(ARQUIVO_ITEM, "rb");
    if (arq_item == NULL) {
        limpar_tela();
        printf("╔════════════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║ Nenhum item cadastrado ainda.                                                             ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════════════════╝\n");
        pausar();
        return;
    }

    // 1 — LER ARQUIVO E INSERIR ORDENADO POR CATEGORIA
    while (fread(&temp, sizeof(Itemcardapio), 1, arq_item) == 1) {

        if (temp.disponivel == 0)
            continue;

        novo = malloc(sizeof(NodeItem));
        novo->dado = temp;
        novo->prox = NULL;

        // insere ordenado por categoria
        if (lista == NULL || strcmp(novo->dado.categoria, lista->dado.categoria) < 0) {
            novo->prox = lista;
            lista = novo;
        } else {
            anter = lista;
            atual = lista->prox;

            while (atual != NULL &&
                   strcmp(novo->dado.categoria, atual->dado.categoria) > 0) {
                anter = atual;
                atual = atual->prox;
            }

            anter->prox = novo;
            novo->prox = atual;
        }
    }
    fclose(arq_item);

    // 2 — EXIBIR IGUAL AO SEU MODELO
    limpar_tela();
    printf("╔════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                         CARDÁPIO                                           ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════════════════╣\n");

    char categoria_atual[30] = "";
    int encontrou = 0;

    atual = lista;
    while (atual != NULL) {

        // mudou categoria
        if (strcmp(categoria_atual, atual->dado.categoria) != 0) {
            if (encontrou)
                printf("╠════════════════════════════════════════════════════════════════════════════════════════════╣\n");

            strcpy(categoria_atual, atual->dado.categoria);
            printf("║   %-89s║\n", categoria_atual);
            printf("║ ------------------------------------------------------------------------------------------ ║\n");
        }

        encontrou = 1;

        char linha[100];
        snprintf(linha, sizeof(linha), "• %-28s R$ %6.2f", atual->dado.nome, atual->dado.preco);
        printf("║ %-93s║\n", linha);

        if (strlen(atual->dado.descricao) > 0) {
            char desc[140];
            snprintf(desc, sizeof(desc), "↳ %s", atual->dado.descricao);
            printf("║    %-90s║\n", desc);
        }

        printf("║                                                                                            ║\n");

        atual = atual->prox;
    }

    if (!encontrou) {
        printf("║ Nenhum item ativo encontrado.                                                             ║\n");
    }

    printf("╚════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    pausar();

    // 3 — LIBERA MEMÓRIA
    while (lista != NULL) {
        atual = lista;
        lista = lista->prox;
        free(atual);
    }
}



void relatorio_cardapio_itens_disponiveis() {

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           ITEM DISPONIVEIS NO CARDAPIO           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    FILE* arq_cardapio = fopen(ARQUIVO_ITEM,"rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }
    while (fread(item, sizeof(Itemcardapio), 1, arq_cardapio) == 1) {
        if(item->disponivel == 1){
        exibir_item(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();
}



void relatorio_cardapio_itens_indisponiveis() {
   limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           ITEM INDISPONIVEIS NO CARDAPIO         ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    FILE* arq_cardapio = fopen(ARQUIVO_ITEM,"rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }
    while (fread(item, sizeof(Itemcardapio), 1, arq_cardapio) == 1) {
        if(item->disponivel == 1){
        exibir_item(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();
}



void procurar_item_por_categoria() {

    char categoria_lida[15];
    Itemcardapio* item = (Itemcardapio*) malloc(sizeof(Itemcardapio));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          PROCURAR ITEM POR CATEGORIA             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    printf("digite a categoria dos item que voce quer ver: ");
    ler_string(categoria_lida, 15);

    limpar_tela();
    FILE* arq_cardapio = fopen(ARQUIVO_ITEM,"rb");
    if (arq_cardapio == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        return;
    }

    while (fread(item, sizeof(Itemcardapio), 1, arq_cardapio) == 1) {
        if (strstr(item->categoria, categoria_lida) != NULL) {
            exibir_item(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();


}



void exibindo_item_do_cardapio_por_preco() {
    FILE *fp;
    NodeItem *lista = NULL;
    NodeItem *novo, *atual, *anter;

    fp = fopen(ARQUIVO_ITEM, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo do cardápio!\n");
        pausar();
        return;
    }

    // Lendo o arquivo e montando a lista ORDENADA
    Itemcardapio temp;

    while (fread(&temp, sizeof(Itemcardapio), 1, fp) == 1) {

        // pula itens indisponíveis
        if (temp.disponivel == 0) 
            continue;

        novo = (NodeItem*) malloc(sizeof(NodeItem));
        novo->dado = temp;
        novo->prox = NULL;

        // Inserção ordenada
        if (lista == NULL || novo->dado.preco < lista->dado.preco) {
            novo->prox = lista;
            lista = novo;
        } else {
            anter = lista;
            atual = lista->prox;

            while (atual != NULL && novo->dado.preco > atual->dado.preco) {
                anter = atual;
                atual = atual->prox;
            }

            anter->prox = novo;
            novo->prox = atual;
        }
    }

    fclose(fp);

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║            ITENS DO CARDÁPIO POR PREÇO           ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");

    // Exibindo lista
    int i = 1;
    atual = lista;
    while (atual != NULL) {

        printf("║  %d) %s\n", i, atual->dado.nome);
        printf("║     Categoria : %s\n", atual->dado.categoria);
        printf("║     Preço     : R$ %.2f\n", atual->dado.preco);
        printf("║     Status    : Disponível\n");
        printf("║--------------------------------------------------║\n");

        atual = atual->prox;
        i++;
    }

    printf("╚══════════════════════════════════════════════════╝\n");
    pausar();

    // Liberando memória
    while (lista != NULL) {
        atual = lista;
        lista = lista->prox;
        free(atual);
    }
}



void relatorio_cardapio() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATÓRIO D0 CARDÁPIO              ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. exibir cardapio                             ║\n");
    printf("║ ► 2. Itens disponíveis                           ║\n");
    printf("║ ► 3. Itens indisponíveis                         ║\n");
    printf("║ ► 4. procurar item por categoria                 ║\n");
    printf("║ ► 5. exibir itens por preço                      ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
    
}



void exibir_todo_o_estoque() {

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          EXIBIR TODOS OS ITENS DO ESTOQUE        ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen(ARQUIVO_ESTOQUE, "rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }


    Nodeproduto* inicio = NULL;
    Nodeproduto* fim = NULL;

    Produto temp;

    while (fread(&temp, sizeof(Produto), 1, arq_estoque) == 1) {

        Nodeproduto* novo = (Nodeproduto*) malloc(sizeof(Nodeproduto));
        novo->dado = temp;
        novo->prox = NULL;

        if (inicio == NULL) {
            inicio = novo;
            fim = novo;
        } else {
            fim->prox = novo;
            fim = novo;
        }
    }

    fclose(arq_estoque);


    Nodeproduto* atual = inicio;
    while (atual != NULL) {
        exibir_item_estoque(&atual->dado);
        atual = atual->prox;
    }

    atual = inicio;
    while (atual != NULL) {
        Nodeproduto* aux = atual;
        atual = atual->prox;
        free(aux);
    }

    pausar();
}



void exibir_itens_com_baixa_quantidade() {
    
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║        ITENS COM BAIXA QUANTIDADE NO ESTOQUE     ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen(ARQUIVO_ESTOQUE,"rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }

    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if(prod->quantidade < 5){
        exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
}



void exibir_itens_indisponiveis_estoque() {
    
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║        ITENS INDISPONIVEIS NO ESTOQUE            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    FILE* arq_estoque = fopen(ARQUIVO_ESTOQUE,"rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }

    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if(prod->ativo == 0){
        exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
}



void exibir_itens_por_nome() {
    char nome_lido[30];
    Produto* prod = (Produto*) malloc(sizeof(Produto));
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          PROCURAR ITEM POR NOME                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    printf("digite o nome dos item que voce quer ver: ");
    ler_string(nome_lido, 30);

    limpar_tela();
    FILE* arq_estoque = fopen(ARQUIVO_ESTOQUE,"rb");
    if (arq_estoque == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        limparBuffer();
        return;
    }
    while (fread(prod, sizeof(Produto), 1, arq_estoque) == 1) {
        if (strstr(prod->nome, nome_lido) != NULL) {
            exibir_item_estoque(prod);
        }
    }
    fclose(arq_estoque);
    free(prod);
    pausar();
}



Nodeproduto* montar_lista_ordenada_por_quantidade() {

    FILE* arq = fopen(ARQUIVO_ESTOQUE, "rb");
    if (!arq) {
        printf("Erro ao abrir arquivo de estoque.\n");
        return NULL;
    }

    Nodeproduto* lista = NULL;
    Nodeproduto* tail = NULL;
    Produto temp;

    // ========== Montagem da lista ==========
    while (fread(&temp, sizeof(Produto), 1, arq) == 1) {

        Nodeproduto* novo = malloc(sizeof(Nodeproduto));
        novo->dado = temp;
        novo->prox = NULL;

        if (lista == NULL) {
            lista = novo;
            tail = novo;
        } else {
            tail->prox = novo;
            tail = novo;
        }
    }

    fclose(arq);

    // =======================================
    // =========== ORDENAÇÃO =================
    // =======================================

    if (!lista) return NULL;

    int trocou;
    Nodeproduto *p;
    Nodeproduto *ultimo = NULL;

    do {
        trocou = 0;
        p = lista;

        while (p->prox != ultimo) {
            if (p->dado.quantidade > p->prox->dado.quantidade) {

                Produto aux = p->dado;
                p->dado = p->prox->dado;
                p->prox->dado = aux;

                trocou = 1;
            }
            p = p->prox;
        }
        ultimo = p;

    } while (trocou);

    return lista;
}



void listar_estoque_por_quantidade() {

    Nodeproduto* lista = montar_lista_ordenada_por_quantidade();

    if (!lista) {
        printf("Nenhum item no estoque.\n");
        pausar();
        return;
    }

    limpar_tela();
    printf("ID   %-25s %-15s %s\n", "Nome", "Categoria", "Quantidade");
    printf("-----------------------------------------------------------------------\n");

    int contador = 1;
    Nodeproduto* p = lista;

    while (p != NULL) {
        printf("%-4d %-25s %-15s %d\n",
               contador,
               p->dado.nome,
               p->dado.categoria,
               p->dado.quantidade);

        p = p->prox;
        contador++;
    }

    // LIBERA A MEMÓRIA AQUI
    liberar_lista(lista);

    pausar();
}



void liberar_lista(Nodeproduto *lista) {
    Nodeproduto *temp;

    while (lista != NULL) {
        temp = lista;
        lista = lista->prox;
        free(temp);
    }
}



void relatorio_estoque() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATÓRIO DO ESTOQUE               ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. exibir estoque completo                     ║\n");
    printf("║ ► 2. Itens com baixa quantidade                  ║\n");
    printf("║ ► 3. Itens indisponíveis                         ║\n");
    printf("║ ► 4. Itens por nome                              ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
    
}   



void relatorio() {
    int opcao, opcao_estoque, opcao_cardapio;
    

    do {
        menu_relatorio();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                do {
                    relatorio_cardapio();
                    scanf("%d", &opcao_cardapio);
                    limparBuffer();

                    switch (opcao_cardapio) {
                        case 1:
                            exibindo_cardapio_por_categoria();
                            break;
                        case 2:
                            relatorio_cardapio_itens_disponiveis();
                            break;
                        case 3:
                            relatorio_cardapio_itens_indisponiveis();
                            break;
                        case 4:
                            procurar_item_por_categoria();
                            break;
                        case 5:
                            exibindo_item_do_cardapio_por_preco();
                            break;
                        case 0:
                            printf("Voltando ao Menu de Relatórios...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao_cardapio != 0);
                break;

            case 2:
                do {
                    relatorio_estoque();
                    scanf("%d", &opcao_estoque);
                    limparBuffer();

                    switch (opcao_estoque) {
                        case 1:
                            exibir_todo_o_estoque();
                            break;
                        case 2:
                            exibir_itens_com_baixa_quantidade();
                            break;
                        case 3:
                            exibir_itens_indisponiveis_estoque();
                            break;
                        case 4:
                            exibir_itens_por_nome();
                            break;
                        case 0:
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }

                } while (opcao_estoque != 0);
                break;
            case 0:
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}