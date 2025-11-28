#ifndef FINANCEIRO_H
#define FINANCEIRO_H

typedef struct {
    char descricao[100];
    char tipo[10]; // "ENTRADA" ou "SAIDA"
    float valor;
    char data[11];
    char categoria[50]; // ex: vendas, ingredientes, manutenção
    int ativo;
} Transacao;

typedef struct NodeTransacao {
    Transacao dado;
    struct NodeTransacao* prox;
} NodeTransacao;


void menu_financeiro();
void financeiro();
void cadastrar_transacao();
void listar_transacoes();
void editar_transacao();
void excluir_transacao();
void relatorio_financeiro();
void exibir_transacao(Transacao* trans);
int confirma_dados_transacao(Transacao* trans);
void gravar_transacao(Transacao* trans);

#endif