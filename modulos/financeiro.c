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
