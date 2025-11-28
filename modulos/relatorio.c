#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "estoque.h"
#include "relatorio.h"
#include "cliente.h"
#include "cardapio.h"
#include "pedido.h"

#define ARQUIVO_ITEM "dados/item_cardapio.dat"
#define ARQUIVO_ESTOQUE "dados/estoque.dat"
#define ARQUIVO_PEDIDOS "dados/pedidos.dat"
#define ARQUIVO_CLIENTES "dados/clientes.dat"


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



///////////////////////////////////////////////////////////////////////////////
// RELATÓRIOS DO CARDAPIO
///////////////////////////////////////////////////////////////////////////////



void exibir_item_listagem(Itemcardapio* item) {
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("| Nome: %s\n", item->nome);
    printf("| Categoria: %s\n", item->categoria);
    printf("| Preço: R$ %.2f\n", item->preco);
    printf("| Descrição: %s\n", item->descricao);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}



NodeItem* montar_lista_itens_ordenados_preco() {

    FILE *fp = fopen(ARQUIVO_ITEM, "rb");
    if (!fp) return NULL;

    NodeItem *lista = NULL;
    NodeItem *novo, *atual, *anter;

    Itemcardapio temp;

    while (fread(&temp, sizeof(Itemcardapio), 1, fp) == 1) {

        if (temp.disponivel == 0)
            continue;

        novo = (NodeItem*) malloc(sizeof(NodeItem));
        novo->dado = temp;
        novo->prox = NULL;

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
    return lista;
}



void exibir_lista_itens(NodeItem *lista) {

    limpar_tela();

    printf("ID   %-25s %-15s %-10s\n", "Nome", "Categoria", "Preço");
    printf("-----------------------------------------------------------------\n");

    int id = 1;
    NodeItem *p = lista;

    while (p != NULL) {
        printf("%-4d %-25s %-15s R$ %.2f\n",
               id,
               p->dado.nome,
               p->dado.categoria,
               p->dado.preco);

        p = p->prox;
        id++;
    }

    pausar();
}



void listar_itens_por_preco() {

    NodeItem *lista = montar_lista_itens_ordenados_preco();

    if (!lista) {
        printf("Nenhum item disponível no cardápio.\n");
        pausar();
        return;
    }

    exibir_lista_itens(lista);

    liberar_lista_cardapio(lista);
}



void liberar_lista_cardapio(NodeItem *lista) {
    NodeItem *aux;

    while (lista != NULL) {
        aux = lista;          // guarda o nó atual
        lista = lista->prox;  // avança
        free(aux);            // libera o nó
    }
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
            if (strlen(desc) > 90)
            desc[90] = '\0';
        
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

    liberar_lista_cardapio(lista);
}



void relatorio_cardapio_itens_disponiveis() {
    relatorio_itens_por_status(1, "ITEMS DISPONÍVEIS NO CARDÁPIO");
}



void relatorio_cardapio_itens_indisponiveis() {
    relatorio_itens_por_status(0, "ITEMS INDISPONÍVEIS NO CARDÁPIO");
}



void relatorio_itens_por_status(int status, const char *titulo) {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║ %50s ║\n", titulo);
    printf("╚══════════════════════════════════════════════════╝\n");

    Itemcardapio *item = malloc(sizeof(Itemcardapio));
    FILE *arq = fopen(ARQUIVO_ITEM, "rb");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de cardapio.\n");
        limparBuffer();
        free(item);
        return;
    }

    while (fread(item, sizeof(Itemcardapio), 1, arq) == 1) {
        if (item->disponivel == status) {
            exibir_item_listagem(item);
        }
    }

    fclose(arq);
    free(item);
    pausar();
}



int strcasestr_custom(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;

    char h[200], n[200];
    int i;

    // copia e converte para minúsculas
    for (i = 0; haystack[i] && i < 199; i++)
        h[i] = tolower(haystack[i]);
    h[i] = '\0';

    for (i = 0; needle[i] && i < 199; i++)
        n[i] = tolower(needle[i]);
    n[i] = '\0';

    // procura substring
    return strstr(h, n) != NULL;
}



void procurar_item_por_categoria() {

    char categoria_lida[40];
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

        if (strcasestr_custom(item->categoria, categoria_lida)) {
            exibir_item_listagem(item);
        }
    }
    fclose(arq_cardapio);
    free(item);
    pausar();


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



/////////////////////////////////////////////////////////////////////////////////////
// RELATÓRIOS DO ESTOQUE
/////////////////////////////////////////////////////////////////////////////////////   



Nodeproduto* montar_lista_estoque() {

    FILE *fp = fopen(ARQUIVO_ESTOQUE, "rb");
    if (!fp) return NULL;

    Nodeproduto *lista = NULL;
    Nodeproduto *ultimo = NULL;
    Nodeproduto *novo;

    Produto temp;

    while (fread(&temp, sizeof(Produto), 1, fp) == 1) {
        novo = (Nodeproduto*) malloc(sizeof(Nodeproduto));
        novo->dado = temp;
        novo->prox = NULL;

        if (lista == NULL) {
            lista = novo;       // primeiro nó
            ultimo = novo;
        } else {
            ultimo->prox = novo; // insere no fim
            ultimo = novo;
        }
    }

    fclose(fp);
    return lista;
}



void ordenar_por_quantidade(Nodeproduto *lista) {
    if (!lista) return;

    int trocou;
    Nodeproduto *p;
    Nodeproduto *ultimo = NULL;

    do {
        trocou = 0;
        p = lista;

        while (p->prox != ultimo) {
            if (p->dado.quantidade > p->prox->dado.quantidade) {
                Produto temp = p->dado;
                p->dado = p->prox->dado;
                p->prox->dado = temp;
                trocou = 1;
            }
            p = p->prox;
        }

        ultimo = p;

    } while (trocou);
}



void exibir_lista_estoque(Nodeproduto *lista) {
    if (!lista) {
        printf("Nenhum item no estoque.\n");
        return;
    }

    printf("ID   %-25s %-15s %-15s %-5s \n", "Nome", "Categoria", "Quantidade", "Validade");
    printf("---------------------------------------------------------------------------------\n");

    Nodeproduto *p = lista;
    int contador = 1;

    while (p != NULL) {
        printf("%-4d %-25s %-15s %d %23s\n",
               contador,
               p->dado.nome,
               p->dado.categoria,
               p->dado.quantidade,
               p->dado.validade);

        p = p->prox;
        contador++;
    }
}



void liberar_lista(Nodeproduto *lista) {
    Nodeproduto *temp;

    while (lista != NULL) {
        temp = lista;
        lista = lista->prox;
        free(temp);
    }
}



void listar_todo_estoque() {
    Nodeproduto *lista = montar_lista_estoque();
    if (!lista) {
        printf("Nenhum item encontrado.\n");
        pausar();
        return;
    }

    limpar_tela();
    exibir_lista_estoque(lista);

    liberar_lista(lista);
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



void listar_estoque_por_quantidade() {
    Nodeproduto *lista = montar_lista_estoque();
    if (!lista) {
        printf("Nenhum item encontrado.\n");
        pausar();
        return;
    }

    ordenar_por_quantidade(lista);

    limpar_tela();
    exibir_lista_estoque(lista);

    liberar_lista(lista);
    pausar();
}



void relatorio_estoque() { 
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATÓRIO DO ESTOQUE               ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Listar todo o estoque                       ║\n");
    printf("║ ► 2. Itens com baixa quantidade                  ║\n");
    printf("║ ► 3. Itens indisponiveis                         ║\n");
    printf("║ ► 4. Procurar item por nome                      ║\n");
    printf("║ ► 5. Listar estoque por quantidade               ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");

}



//////////////////////////////////////////////////////////////////////////////////////////
// RELATÓRIOS CLIENTES
//////////////////////////////////////////////////////////////////////////////////////////



void relatorio_clientes() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               RELATORIO CLIENTES                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Exibir todos os clientes                    ║\n");
    printf("║ ► 2. Clientes com pedidos ativos                 ║\n");
    printf("║ ► 3. ultimo pedido por cliente                   ║\n");
    printf("║ ► 4. Procurar cliente por nome                   ║\n");
    printf("║ ► 5. Listar clientes por ordem alfabética        ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");

} 



void relatorio_clientes_com_ultimo_pedido() {
    FILE *arq_cli, *arq_ped;
    Cliente cli;
    Pedido ped;
    Pedido ultimo;

    int achou;

    limpar_tela();

    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                         ULTIMO PEDIDO                    ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n\n");

    arq_cli = fopen("dados/clientes.dat", "rb");
    if (!arq_cli) {
        printf("Nenhum cliente cadastrado.\n");
        pausar();
        return;
    }

    while (fread(&cli, sizeof(Cliente), 1, arq_cli) == 1) {

        if (cli.status == 0) continue;

        printf("Cliente: %s\n", cli.nome);
        printf("CPF: %s\n", cli.cpf);
        printf("Telefone: %s\n\n", cli.telefone);

        arq_ped = fopen("dados/pedidos.dat", "rb");
        if (!arq_ped) {
            printf("   Nenhum pedido encontrado.\n");
            printf("----------------------------------------------------------\n\n");
            continue;
        }

        achou = 0;

        while (fread(&ped, sizeof(Pedido), 1, arq_ped) == 1) {
            if (ped.ativo == 1 && strcmp(ped.nome_cliente, cli.nome) == 0) {
                ultimo = ped;
                achou = 1;
            }
        }

        fclose(arq_ped);

        if (!achou) {
            printf("   Nenhum pedido realizado por este cliente.\n");
            printf("----------------------------------------------------------\n\n");
            continue;
        }

        printf("Último Pedido: #%d\n", ultimo.numero_pedido);
        printf("Data: %s\n", ultimo.data);
        printf("Valor Total: R$ %.2f\n", ultimo.valor_total);
        printf("Status: %s\n", ultimo.status);

        printf("----------------------------------------------------------\n\n");
    }

    fclose(arq_cli);
    pausar();
}



///////////////////////////////////////////////////////////////////////////////
// RELATÓRIOS DOS PEDIDOS
///////////////////////////////////////////////////////////////////////////////



void exibir_pedidos_por_status() {
    FILE *arq;
    Pedido ped;
    char status_busca[20];
    int contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           RELATÓRIO DE PEDIDOS POR STATUS        ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("Status disponíveis:\n");
    printf("1. Em preparo\n");
    printf("2. Saiu para entrega\n");
    printf("3. Entregue\n\n");
    printf("Digite o status: ");
    ler_string(status_busca, sizeof(status_busca));

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("\nNenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║         PEDIDOS COM STATUS: %-20s║\n", status_busca);
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1 && strcmp(ped.status, status_busca) == 0) {
            contador++;
            printf("Pedido #%d\n", ped.numero_pedido);
            printf("Cliente: %s\n", ped.nome_cliente);
            printf("Telefone: %s\n", ped.telefone_cliente);
            printf("Tipo: %s\n", ped.eh_delivery ? "DELIVERY" : "CONSUMO NO LOCAL");
            printf("Data: %s\n", ped.data);
            printf("Valor: R$ %.2f\n", ped.valor_total);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhum pedido encontrado com esse status.\n");
    } else {
        printf("Total: %d pedidos\n", contador);
    }

    pausar();
}



void relatorio_delivery_vs_local() {
    FILE *arq;
    Pedido ped;
    int total_delivery = 0, total_local = 0;
    float valor_delivery = 0.0, valor_local = 0.0;
    float taxa_total = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║       RELATÓRIO: DELIVERY VS CONSUMO LOCAL       ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            if (ped.eh_delivery) {
                total_delivery++;
                valor_delivery += ped.valor_total;
                taxa_total += ped.taxa_entrega;
            } else {
                total_local++;
                valor_local += ped.valor_total;
            }
        }
    }

    fclose(arq);

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                    DELIVERY                      ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Quantidade de Pedidos: %-26d║\n", total_delivery);
    printf("║ Faturamento: R$ %-33.2f║\n", valor_delivery);
    printf("║ Total em Taxas: R$ %-30.2f║\n", taxa_total);
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 CONSUMO NO LOCAL                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Quantidade de Pedidos: %-26d║\n", total_local);
    printf("║ Faturamento: R$ %-33.2f║\n", valor_local);
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                  RESUMO GERAL                    ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Total de Pedidos: %-30d║\n", total_delivery + total_local);
    printf("║ Faturamento Total: R$ %-27.2f║\n", valor_delivery + valor_local);
    printf("╚══════════════════════════════════════════════════╝\n");

    pausar();
}



void relatorio_itens_mais_pedidos() {
    FILE *arq;
    Pedido ped;
    
    typedef struct {
        char nome[50];
        int quantidade_total;
        float valor_total;
    } ItemRelatorio;
    
    ItemRelatorio itens[100];
    int total_itens = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           RELATÓRIO: ITENS MAIS PEDIDOS          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            for (int i = 0; i < ped.total_itens; i++) {
                int encontrado = 0;
                
                for (int j = 0; j < total_itens; j++) {
                    if (strcmp(itens[j].nome, ped.itens[i].item.nome) == 0) {
                        itens[j].quantidade_total += ped.itens[i].quantidade;
                        itens[j].valor_total += ped.itens[i].item.preco * ped.itens[i].quantidade;
                        encontrado = 1;
                        break;
                    }
                }
                
                if (!encontrado && total_itens < 100) {
                    strcpy(itens[total_itens].nome, ped.itens[i].item.nome);
                    itens[total_itens].quantidade_total = ped.itens[i].quantidade;
                    itens[total_itens].valor_total = ped.itens[i].item.preco * ped.itens[i].quantidade;
                    total_itens++;
                }
            }
        }
    }

    fclose(arq);

    if (total_itens == 0) {
        printf("Nenhum item encontrado.\n");
        pausar();
        return;
    }

    for (int i = 0; i < total_itens - 1; i++) {
        for (int j = 0; j < total_itens - i - 1; j++) {
            if (itens[j].quantidade_total < itens[j + 1].quantidade_total) {
                ItemRelatorio temp = itens[j];
                itens[j] = itens[j + 1];
                itens[j + 1] = temp;
            }
        }
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    for (int i = 0; i < total_itens; i++) {
        printf("%d. %s\n", i + 1, itens[i].nome);
        printf("   Quantidade vendida: %d unidades\n", itens[i].quantidade_total);
        printf("   Faturamento: R$ %.2f\n", itens[i].valor_total);
        printf("\n");
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    pausar();
}



void relatorio_historico_cliente() {
    FILE *arq_ped, *arq_cli;
    Pedido ped;
    Cliente cli;
    char telefone_busca[15];
    int contador = 0;
    float total_gasto = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║        RELATÓRIO: HISTÓRICO DO CLIENTE           ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Telefone do Cliente: ");
    ler_string(telefone_busca, sizeof(telefone_busca));

    arq_cli = fopen(ARQUIVO_CLIENTES, "rb");
    if (arq_cli == NULL) {
        printf("\nNenhum cliente cadastrado.\n");
        pausar();
        return;
    }

    int cliente_encontrado = 0;
    while (fread(&cli, sizeof(Cliente), 1, arq_cli) == 1) {
        if (cli.status == 1 && strcmp(cli.telefone, telefone_busca) == 0) {
            cliente_encontrado = 1;
            limpar_tela();
            printf("╔══════════════════════════════════════════════════╗\n");
            printf("║              DADOS DO CLIENTE                    ║\n");
            printf("╠══════════════════════════════════════════════════╣\n");
            printf("║ Nome: %-43s║\n", cli.nome);
            printf("║ Telefone: %-39s║\n", cli.telefone);
            printf("║ Endereço: %-39s║\n", cli.endereco);
            printf("╚══════════════════════════════════════════════════╝\n\n");
            break;
        }
    }
    fclose(arq_cli);

    if (!cliente_encontrado) {
        printf("\nCliente não encontrado.\n");
        pausar();
        return;
    }

    arq_ped = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq_ped == NULL) {
        printf("Nenhum pedido cadastrado.\n");
        pausar();
        return;
    }

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              HISTÓRICO DE PEDIDOS                ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&ped, sizeof(Pedido), 1, arq_ped) == 1) {
        if (ped.ativo == 1 && strcmp(ped.telefone_cliente, telefone_busca) == 0) {
            contador++;
            total_gasto += ped.valor_total;

            printf("Pedido #%d\n", ped.numero_pedido);
            printf("Data: %s\n", ped.data);
            printf("Status: %s\n", ped.status);
            printf("Tipo: %s\n", ped.eh_delivery ? "DELIVERY" : "CONSUMO NO LOCAL");
            printf("Itens: %d\n", ped.total_itens);
            printf("Valor: R$ %.2f\n", ped.valor_total);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }

    fclose(arq_ped);

    if (contador == 0) {
        printf("Este cliente ainda não fez pedidos.\n");
    } else {
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║                    RESUMO                        ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║ Total de Pedidos: %-30d║\n", contador);
        printf("║ Total Gasto: R$ %-33.2f║\n", total_gasto);
        printf("║ Ticket Médio: R$ %-31.2f║\n", contador > 0 ? total_gasto / contador : 0.0);
        printf("╚══════════════════════════════════════════════════╝\n");
    }

    pausar();
}



void relatorio_pedidos_por_data() {
    FILE *arq;
    Pedido ped;
    char data_busca[11];
    int contador = 0;
    float total_dia = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          RELATÓRIO: PEDIDOS POR DATA             ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Data (DD/MM/AAAA): ");
    ler_string(data_busca, sizeof(data_busca));

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("\nNenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║         PEDIDOS DO DIA: %-24s║\n", data_busca);
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1 && strcmp(ped.data, data_busca) == 0) {
            contador++;
            total_dia += ped.valor_total;

            printf("Pedido #%d\n", ped.numero_pedido);
            printf("Cliente: %s\n", ped.nome_cliente);
            printf("Tipo: %s\n", ped.eh_delivery ? "DELIVERY" : "CONSUMO NO LOCAL");
            printf("Status: %s\n", ped.status);
            printf("Valor: R$ %.2f\n", ped.valor_total);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhum pedido encontrado nesta data.\n");
    } else {
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║                RESUMO DO DIA                     ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║ Total de Pedidos: %-30d║\n", contador);
        printf("║ Faturamento: R$ %-33.2f║\n", total_dia);
        printf("║ Ticket Médio: R$ %-31.2f║\n", contador > 0 ? total_dia / contador : 0.0);
        printf("╚══════════════════════════════════════════════════╝\n");
    }

    pausar();
}



void relatorio_pedidos() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             RELATÓRIO DE PEDIDOS                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Todos os Pedidos                            ║\n");
    printf("║ ► 2. Pedidos por Status                          ║\n");
    printf("║ ► 3. Delivery vs Consumo Local                   ║\n");
    printf("║ ► 4. Itens Mais Pedidos                          ║\n");
    printf("║ ► 5. Histórico de Cliente                        ║\n");
    printf("║ ► 6. Pedidos por Data                            ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}



void relatorio() {
    int opcao, opcao_estoque, opcao_cardapio, opcao_clientes;
    

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
                            listar_itens_por_preco();
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
                            listar_todo_estoque();
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
                        case 5:
                            listar_estoque_por_quantidade();
                            break;
                        case 0:
                            break;
                        default:
                      printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao_estoque != 0);
                break;
            case 3:
                do{
                    relatorio_clientes();
                    scanf("%d", &opcao_clientes);
                    limparBuffer();

                    switch (opcao_clientes) {
                        case 1:
                            break;
                        case 2:
                            break;
                        case 3:
                            relatorio_clientes_com_ultimo_pedido();
                            break;
                        case 4:
                            break;
                        case 5:
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao_clientes != 0);
                break;
            case 4:
                do {
                    relatorio_pedidos();
                    scanf("%d", &opcao);
                    limparBuffer();

                    switch (opcao) {
                        case 1:
                            listar_pedidos();
                            break;
                        case 2:
                            exibir_pedidos_por_status();
                            break;
                        case 3:
                            relatorio_delivery_vs_local();
                            break;
                        case 4:
                            relatorio_itens_mais_pedidos();
                            break;
                        case 5:
                            relatorio_historico_cliente();
                            break;
                        case 6:
                            relatorio_pedidos_por_data();
                            break;
                        case 0:
                            printf("Voltando ao Menu de Relatórios...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcao != 0);
                break;
            case 0:
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
