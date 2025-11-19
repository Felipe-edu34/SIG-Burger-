#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "financeiro.h"

#define ARQUIVO_FINANCEIRO "dados/financeiro.dat"

void menu_financeiro() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               MÓDULO FINANCEIRO                  ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Registrar Transação                         ║\n");
    printf("║ ► 2. Listar Transações                           ║\n");
    printf("║ ► 3. Editar Transação                            ║\n");
    printf("║ ► 4. Excluir Transação                           ║\n");
    printf("║ ► 5. Relatório Financeiro                        ║\n");
    printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}

int confirma_dados_transacao(Transacao* trans) {
    char confirm;
    limpar_tela();
    exibir_transacao(trans);
    printf("\nOs dados da transação estão corretos? (S/N): ");
    scanf(" %c", &confirm);
    limparBuffer();

    if (confirm == 'S' || confirm == 's') {
        return 1;
    } else {
        return 0;
    }
}

void exibir_transacao(Transacao* trans) {
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║            DETALHES DA TRANSAÇÃO                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Descrição:   %s\n", trans->descricao);
    printf("║ Tipo:        %s\n", trans->tipo);
    printf("║ Categoria:   %s\n", trans->categoria);
    printf("║ Valor:       R$ %.2f\n", trans->valor);
    printf("║ Data:        %s\n", trans->data);
    printf("╚══════════════════════════════════════════════════╝\n");
}

void gravar_transacao(Transacao* trans) {
    FILE *arq = fopen(ARQUIVO_FINANCEIRO, "ab");
    if (arq == NULL) {
        return;
    }

    fwrite(trans, sizeof(Transacao), 1, arq);
    fclose(arq);
}

void cadastrar_transacao() {
    Transacao* trans = (Transacao*) malloc(sizeof(Transacao));

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║            REGISTRAR NOVA TRANSAÇÃO              ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    printf("► Descrição: ");
    ler_string(trans->descricao, sizeof(trans->descricao));

    printf("► Tipo (Receita/Despesa): ");
    ler_string(trans->tipo, sizeof(trans->tipo));

    printf("► Categoria: ");
    ler_string(trans->categoria, sizeof(trans->categoria));

    printf("► Valor (R$): ");
    if (scanf("%f", &trans->valor) != 1) {
        limparBuffer();
        printf("Valor inválido.\n");
        free(trans);
        pausar();
        return;
    }
    limparBuffer();

    printf("► Data (DD/MM/AAAA): ");
    ler_string(trans->data, sizeof(trans->data));

    trans->ativo = 1;

    if (!confirma_dados_transacao(trans)) {
        printf("\nCadastro cancelado pelo usuário.\n");
        free(trans);
        pausar();
        return;
    }

    gravar_transacao(trans);
    printf("\n Transação registrada com sucesso!\n");
    free(trans);
    pausar();
}

void listar_transacoes() {
    FILE *arq;
    Transacao trans;
    int contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              LISTA DE TRANSAÇÕES                 ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("Nenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            contador++;
            printf("Transação %d\n", contador);
            printf("Tipo: %s\n", trans.tipo);
            printf("Descrição: %s\n", trans.descricao);
            printf("Categoria: %s\n", trans.categoria);
            printf("Valor: R$ %.2f\n", trans.valor);
            printf("Data: %s\n", trans.data);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhuma transação ativa encontrada.\n");
    } else {
        printf("Total de transações: %d\n", contador);
    }

    pausar();
}

void editar_transacao() {
    FILE *arq;
    Transacao trans;
    int numero, contador = 0;
    long pos_arquivo;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               EDITAR TRANSAÇÃO                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("Nenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    printf("Transações cadastradas:\n\n");
    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            contador++;
            printf(" %d - %s - %s (R$ %.2f)\n",
                   contador, trans.tipo, trans.descricao, trans.valor);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhuma transação ativa encontrada.\n");
        pausar();
        return;
    }

    printf("\nDigite o número da transação: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq = fopen(ARQUIVO_FINANCEIRO, "r+b");
    contador = 0;
    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            contador++;
            if (contador == numero) {
                pos_arquivo = ftell(arq) - sizeof(Transacao);
                break;
            }
        }
    }

    printf("\nEditando transação: %s\n", trans.descricao);
    printf("----------------------------------------------------\n");

    printf("► Nova Descrição: ");
    ler_string(trans.descricao, sizeof(trans.descricao));

    printf("► Novo Tipo (Receita/Despesa): ");
    ler_string(trans.tipo, sizeof(trans.tipo));

    printf("► Nova Categoria: ");
    ler_string(trans.categoria, sizeof(trans.categoria));

    printf("► Novo Valor (R$): ");
    if (scanf("%f", &trans.valor) != 1) {
        limparBuffer();
        printf("Valor inválido.\n");
        fclose(arq);
        pausar();
        return;
    }
    limparBuffer();

    printf("► Nova Data (DD/MM/AAAA): ");
    ler_string(trans.data, sizeof(trans.data));

    fseek(arq, pos_arquivo, SEEK_SET);
    fwrite(&trans, sizeof(Transacao), 1, arq);
    fclose(arq);

    printf("\n Transação atualizada com sucesso!\n");
    pausar();
}

void excluir_transacao() {
    FILE *arq;
    Transacao trans;
    int numero, contador = 0;
    long pos_arquivo;
    char confirma;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              EXCLUIR TRANSAÇÃO                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("Nenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    printf("Transações cadastradas:\n\n");
    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            contador++;
            printf(" %d - %s - %s (R$ %.2f)\n",
                   contador, trans.tipo, trans.descricao, trans.valor);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhuma transação ativa encontrada.\n");
        pausar();
        return;
    }

    printf("\nDigite o número da transação: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq = fopen(ARQUIVO_FINANCEIRO, "r+b");
    contador = 0;
    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            contador++;
            if (contador == numero) {
                pos_arquivo = ftell(arq) - sizeof(Transacao);
                break;
            }
        }
    }

    printf("\nTransação: %s - %s\n", trans.tipo, trans.descricao);
    printf("Valor: R$ %.2f\n\n", trans.valor);
    printf("Confirmar exclusão? (s/n): ");
    scanf(" %c", &confirma);
    limparBuffer();

    if (confirma == 's' || confirma == 'S') {
        trans.ativo = 0;
        fseek(arq, pos_arquivo, SEEK_SET);
        fwrite(&trans, sizeof(Transacao), 1, arq);
        printf("\n Transação excluída com sucesso!\n");
    } else {
        printf("\nExclusão cancelada.\n");
    }

    fclose(arq);
    pausar();
}

void relatorio_financeiro() {
    FILE *arq;
    Transacao trans;
    float total_receitas = 0.0;
    float total_despesas = 0.0;
    float saldo = 0.0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║             RELATÓRIO FINANCEIRO                 ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_FINANCEIRO, "rb");
    if (arq == NULL) {
        printf("Nenhuma transação cadastrada ainda.\n");
        pausar();
        return;
    }

    while (fread(&trans, sizeof(Transacao), 1, arq) == 1) {
        if (trans.ativo == 1) {
            if (strcmp(trans.tipo, "Receita") == 0 || strcmp(trans.tipo, "receita") == 0) {
                total_receitas += trans.valor;
            } else if (strcmp(trans.tipo, "Despesa") == 0 || strcmp(trans.tipo, "despesa") == 0) {
                total_despesas += trans.valor;
            }
        }
    }

    fclose(arq);

    saldo = total_receitas - total_despesas;

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                     RESUMO                       ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ Total de Receitas:    R$ %10.2f             ║\n", total_receitas);
    printf("║ Total de Despesas:    R$ %10.2f             ║\n", total_despesas);
    printf("║ ------------------------------------------------ ║\n");
    printf("║ Saldo:                R$ %10.2f             ║\n", saldo);
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    if (saldo > 0) {
        printf("\n✓ Resultado: LUCRO\n");
    } else if (saldo < 0) {
        printf("\n✗ Resultado: PREJUÍZO\n");
    } else {
        printf("\n= Resultado: EQUILIBRADO\n");
    }

    pausar();
}
