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
#include "financeiro.h"

#define ARQUIVO_ITEM "dados/item_cardapio.dat"
#define ARQUIVO_ESTOQUE "dados/estoque.dat"
#define ARQUIVO_PEDIDOS "dados/pedidos.dat"
#define ARQUIVO_CLIENTES "dados/clientes.dat"
#define ARQUIVO_FINANCEIRO "dados/financeiro.dat"


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
            exibir_item(item);
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
            exibir_item(item);
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


///////////////////////////////////////////////////////////////////////////////
// RELATÓRIOS FINANCEIROS
///////////////////////////////////////////////////////////////////////////////

void relatorio_financeiro_menu() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             RELATÓRIO FINANCEIRO                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Transações por Período                      ║\n");
    printf("║ ► 2. Transações por Categoria                    ║\n");
    printf("║ ► 3. Maiores Entradas                            ║\n");
    printf("║ ► 4. Maiores Saídas                              ║\n");
    printf("║ ► 5. Fluxo de Caixa Mensal                       ║\n");
    printf("║ ► 6. Comparativo: Pedidos vs Transações          ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}

void relatorio_transacoes_periodo() {
    FILE *arq;
    Transacao trans;
    char data_inicio[11], data_fim[11];
    float total_entradas = 0.0;
    float total_saidas = 0.0;
    int contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║         TRANSAÇÕES POR PERÍODO                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Data Inicial (DD/MM/AAAA): ");
    ler_string(data_inicio, sizeof(data_inicio));

    printf("► Data Final (DD/MM/AAAA): ");
    ler_string(data_fim, sizeof(data_fim));

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("\nNenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║    TRANSAÇÕES DE %s ATÉ %s    ║\n", data_inicio, data_fim);
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1 && 
            strcmp(trans.data, data_inicio) >= 0 && 
            strcmp(trans.data, data_fim) <= 0) {
            
            contador++;
            printf("%d. %s\n", contador, trans.descricao);
            printf("   Tipo: %s\n", trans.tipo);
            printf("   Categoria: %s\n", trans.categoria);
            printf("   Valor: R$ %.2f\n", trans.valor);
            printf("   Data: %s\n", trans.data);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

            if (strcmp(trans.tipo, "ENTRADA") == 0 || strcmp(trans.tipo, "entrada") == 0) {
                total_entradas += trans.valor;
            } else {
                total_saidas += trans.valor;
            }
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhuma transação encontrada neste período.\n");
    } else {
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║                RESUMO DO PERÍODO                 ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║ Total de Transações: %-28d║\n", contador);
        printf("║ Total Entradas: R$ %-30.2f║\n", total_entradas);
        printf("║ Total Saídas: R$ %-32.2f║\n", total_saidas);
        printf("║ Saldo: R$ %-39.2f║\n", total_entradas - total_saidas);
        printf("╚══════════════════════════════════════════════════╝\n");
    }

    pausar();
}

void relatorio_transacoes_categoria() {
    FILE *arq;
    Transacao trans;
    
    typedef struct {
        char nome[50];
        float total_entrada;
        float total_saida;
        int quantidade;
    } CategoriaRelatorio;
    
    CategoriaRelatorio categorias[50];
    int total_categorias = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║         TRANSAÇÕES POR CATEGORIA                 ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("Nenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            int encontrado = 0;
            
            for (int i = 0; i < total_categorias; i++) {
                if (strcmp(categorias[i].nome, trans.categoria) == 0) {
                    categorias[i].quantidade++;
                    if (strcmp(trans.tipo, "ENTRADA") == 0 || strcmp(trans.tipo, "entrada") == 0) {
                        categorias[i].total_entrada += trans.valor;
                    } else {
                        categorias[i].total_saida += trans.valor;
                    }
                    encontrado = 1;
                    break;
                }
            }
            
            if (!encontrado && total_categorias < 50) {
                strcpy(categorias[total_categorias].nome, trans.categoria);
                categorias[total_categorias].quantidade = 1;
                if (strcmp(trans.tipo, "ENTRADA") == 0 || strcmp(trans.tipo, "entrada") == 0) {
                    categorias[total_categorias].total_entrada = trans.valor;
                    categorias[total_categorias].total_saida = 0.0;
                } else {
                    categorias[total_categorias].total_entrada = 0.0;
                    categorias[total_categorias].total_saida = trans.valor;
                }
                total_categorias++;
            }
        }
    }

    fclose(arq);

    if (total_categorias == 0) {
        printf("Nenhuma categoria encontrada.\n");
        pausar();
        return;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    for (int i = 0; i < total_categorias; i++) {
        float saldo = categorias[i].total_entrada - categorias[i].total_saida;
        
        printf("Categoria: %s\n", categorias[i].nome);
        printf("Quantidade de transações: %d\n", categorias[i].quantidade);
        printf("Total Entradas: R$ %.2f\n", categorias[i].total_entrada);
        printf("Total Saídas: R$ %.2f\n", categorias[i].total_saida);
        printf("Saldo: R$ %.2f", saldo);
        
        if (saldo > 0) printf(" (Positivo)\n");
        else if (saldo < 0) printf(" (Negativo)\n");
        else printf(" (Neutro)\n");
        
        printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
    }

    pausar();
}

NodeTransacao* montar_lista_entradas_ordenadas() {
    FILE *fp = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (!fp) return NULL;

    NodeTransacao *lista = NULL;
    NodeTransacao *novo, *atual, *anter;
    Transacao temp;

    while (fread(&temp, sizeof(Transacao), 1, fp) == 1) {
        if (temp.ativo == 0 || 
            (strcmp(temp.tipo, "ENTRADA") != 0 && strcmp(temp.tipo, "entrada") != 0))
            continue;

        novo = (NodeTransacao*) malloc(sizeof(NodeTransacao));
        novo->dado = temp;
        novo->prox = NULL;

        // Insere ordenado por valor (MAIOR para MENOR)
        if (lista == NULL || novo->dado.valor > lista->dado.valor) {
            novo->prox = lista;
            lista = novo;
        } else {
            anter = lista;
            atual = lista->prox;

            while (atual != NULL && novo->dado.valor < atual->dado.valor) {
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

void liberar_lista_transacoes(NodeTransacao *lista) {
    NodeTransacao *aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

void relatorio_maiores_entradas() {
    NodeTransacao *lista;
    NodeTransacao *p;
    int contador = 0;
    float soma_top = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║            MAIORES ENTRADAS                      ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    lista = montar_lista_entradas_ordenadas();

    if (!lista) {
        printf("Nenhuma entrada encontrada.\n");
        pausar();
        return;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    p = lista;
    while (p != NULL && contador < 10) {
        contador++;
        printf("%d. %s\n", contador, p->dado.descricao);
        printf("   Categoria: %s\n", p->dado.categoria);
        printf("   Valor: R$ %.2f\n", p->dado.valor);
        printf("   Data: %s\n", p->dado.data);
        printf("\n");

        soma_top += p->dado.valor;
        p = p->prox;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Top %d entradas somam: R$ %.2f\n", contador, soma_top);

    liberar_lista_transacoes(lista);
    pausar();
}

NodeTransacao* montar_lista_saidas_ordenadas() {
    FILE *fp = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (!fp) return NULL;

    NodeTransacao *lista = NULL;
    NodeTransacao *novo, *atual, *anter;
    Transacao temp;

    while (fread(&temp, sizeof(Transacao), 1, fp) == 1) {
        if (temp.ativo == 0 || 
            (strcmp(temp.tipo, "SAIDA") != 0 && strcmp(temp.tipo, "saida") != 0))
            continue;

        novo = (NodeTransacao*) malloc(sizeof(NodeTransacao));
        novo->dado = temp;
        novo->prox = NULL;

        // Insere ordenado por valor (MAIOR para MENOR)
        if (lista == NULL || novo->dado.valor > lista->dado.valor) {
            novo->prox = lista;
            lista = novo;
        } else {
            anter = lista;
            atual = lista->prox;

            while (atual != NULL && novo->dado.valor < atual->dado.valor) {
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

void relatorio_maiores_saidas() {
    NodeTransacao *lista;
    NodeTransacao *p;
    int contador = 0;
    float soma_top = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             MAIORES SAÍDAS                       ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    lista = montar_lista_saidas_ordenadas();

    if (!lista) {
        printf("Nenhuma saída encontrada.\n");
        pausar();
        return;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    p = lista;
    while (p != NULL && contador < 10) {
        contador++;
        printf("%d. %s\n", contador, p->dado.descricao);
        printf("   Categoria: %s\n", p->dado.categoria);
        printf("   Valor: R$ %.2f\n", p->dado.valor);
        printf("   Data: %s\n", p->dado.data);
        printf("\n");

        soma_top += p->dado.valor;
        p = p->prox;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Top %d saídas somam: R$ %.2f\n", contador, soma_top);

    liberar_lista_transacoes(lista);
    pausar();
}

void relatorio_fluxo_caixa_mensal() {
    FILE *arq;
    Transacao trans;
    char mes_ano[8];
    float entradas_mes = 0.0;
    float saidas_mes = 0.0;
    int contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║          FLUXO DE CAIXA MENSAL                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Mês/Ano (MM/AAAA): ");
    ler_string(mes_ano, sizeof(mes_ano));

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("\nNenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║       FLUXO DE CAIXA - %s                ║\n", mes_ano);
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            char data_mes[8];
            strncpy(data_mes, trans.data + 3, 7);
            data_mes[7] = '\0';

            if (strcmp(data_mes, mes_ano) == 0) {
                contador++;
                printf("%d. [%s] %s\n", contador, trans.data, trans.descricao);
                printf("   %s - %s: R$ %.2f\n", 
                       trans.tipo, trans.categoria, trans.valor);
                printf("\n");

                if (strcmp(trans.tipo, "ENTRADA") == 0 || strcmp(trans.tipo, "entrada") == 0) {
                    entradas_mes += trans.valor;
                } else {
                    saidas_mes += trans.valor;
                }
            }
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhuma transação encontrada neste mês.\n");
    } else {
        float saldo_mes = entradas_mes - saidas_mes;

        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║              RESUMO DO MÊS                       ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║ Transações: %-37d║\n", contador);
        printf("║ Entradas: R$ %-36.2f║\n", entradas_mes);
        printf("║ Saídas: R$ %-38.2f║\n", saidas_mes);
        printf("║ ------------------------------------------------ ║\n");
        printf("║ Saldo do Mês: R$ %-32.2f║\n", saldo_mes);
        printf("╚══════════════════════════════════════════════════╝\n");

        if (saldo_mes > 0) {
            printf("\n✓ Mês positivo (Lucro)\n");
        } else if (saldo_mes < 0) {
            printf("\n✗ Mês negativo (Prejuízo)\n");
        } else {
            printf("\n= Mês equilibrado\n");
        }
    }

    pausar();
}

void relatorio_comparativo_pedidos_transacoes() {
    FILE *arq_ped, *arq_fin;
    Pedido ped;
    Transacao trans;
    float total_pedidos = 0.0;
    float total_entradas = 0.0;
    int qtd_pedidos = 0;
    int qtd_entradas = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║      COMPARATIVO: PEDIDOS VS TRANSAÇÕES          ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq_ped = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq_ped != NULL) {
        while (fread(&ped, sizeof(Pedido), 1, arq_ped) == 1) {
            if (ped.ativo == 1) {
                total_pedidos += ped.valor_total;
                qtd_pedidos++;
            }
        }
        fclose(arq_ped);
    }

    arq_fin = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq_fin != NULL) {
        while (fread(&trans, sizeof(Transacao), 1, arq_fin) == 1) {
            if (trans.ativo == 1 && 
                (strcmp(trans.tipo, "ENTRADA") == 0 || strcmp(trans.tipo, "entrada") == 0)) {
                total_entradas += trans.valor;
                qtd_entradas++;
            }
        }
        fclose(arq_fin);
    }

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 PEDIDOS                          ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Quantidade: %-37d║\n", qtd_pedidos);
    printf("║ Valor Total: R$ %-33.2f║\n", total_pedidos);
    if (qtd_pedidos > 0) {
        printf("║ Ticket Médio: R$ %-32.2f║\n", total_pedidos / qtd_pedidos);
    }
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║            TRANSAÇÕES (ENTRADAS)                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Quantidade: %-37d║\n", qtd_entradas);
    printf("║ Valor Total: R$ %-33.2f║\n", total_entradas);
    if (qtd_entradas > 0) {
        printf("║ Média: R$ %-39.2f║\n", total_entradas / qtd_entradas);
    }
    printf("╚══════════════════════════════════════════════════╝\n\n");

    float diferenca = total_pedidos - total_entradas;

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                  ANÁLISE                         ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Diferença: R$ %-35.2f║\n", diferenca);

    if (diferenca > 0) {
        printf("║ Status: Pedidos > Entradas (verificar lançamentos) ║\n");
    } else if (diferenca < 0) {
        printf("║ Status: Entradas > Pedidos (verificar origem)      ║\n");
    } else {
        printf("║ Status: Valores coincidem                          ║\n");
    }

    printf("╚══════════════════════════════════════════════════╝\n");

    pausar();
}


void relatorio() {
    int opcao, opcao_estoque, opcao_cardapio, opcao_clientes, opcao_financeiro;
    

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
            case 5:
                do {
                    relatorio_financeiro_menu();
                    scanf("%d", &opcao_financeiro);
                    limparBuffer();

                    switch (opcao_financeiro) {
                        case 1:
                            relatorio_transacoes_periodo();
                            break;

                        case 2:
                            relatorio_transacoes_categoria();
                            break;

                        case 3:
                            relatorio_maiores_entradas();
                            break;

                        case 4:
                            relatorio_maiores_saidas();
                            break;

                        case 5:
                            relatorio_fluxo_caixa_mensal();
                            break;

                        case 6:
                            relatorio_comparativo_pedidos_transacoes();
                            break;

                        case 0:
                            printf("Voltando ao Menu de Relatórios...\n");
                            break;

                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }

                } while (opcao_financeiro != 0);
                break;
            case 0:
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}
