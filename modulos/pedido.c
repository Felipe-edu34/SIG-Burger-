#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pedido.h"
#include "cardapio.h"
#include "leitura.h"
#include "cliente.h"

#define ARQUIVO_PEDIDOS "dados/pedidos.dat"
#define ARQUIVO_CLIENTES "dados/clientes.dat"

void menu_pedidos() {
    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 MÓDULO PEDIDOS                   ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║                                                  ║\n");
    printf("║ ► 1. Cadastrar Pedido                            ║\n");
    printf("║ ► 2. Listar Pedidos                              ║\n");
    printf("║ ► 3. Editar Pedido                               ║\n");
    printf("║ ► 4. Pesquisar Pedido                            ║\n");
    printf("║ ► 5. Excluir Pedido                              ║\n");
    printf("║ ► 0. Voltar ao Menu Principal                    ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}

int gerar_numero_pedido() {
    FILE *arq;
    Pedido ped;
    int maior_numero = 0;

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        return 1;
    }

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.numero_pedido > maior_numero) {
            maior_numero = ped.numero_pedido;
        }
    }
    fclose(arq);

    return maior_numero + 1;
}

int confirma_dados_pedido(Pedido* ped) {
    char confirm;
    limpar_tela();
    exibir_pedido(ped);
    printf("\nOs dados do pedido estão corretos? (S/N): ");
    scanf(" %c", &confirm);
    limparBuffer();

    if (confirm == 'S' || confirm == 's') {
        return 1;
    } else {
        return 0;
    }
}

void exibir_pedido(Pedido* ped) {
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              DETALHES DO PEDIDO                  ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Número:      #%d\n", ped->numero_pedido);
    printf("║ Cliente:     %s\n", ped->nome_cliente);
    printf("║ Telefone:    %s\n", ped->telefone_cliente);
    
    if (ped->eh_delivery) {
        printf("║ Tipo:        DELIVERY\n");
        printf("║ Endereço:    %s\n", ped->endereco_entrega);
    } else {
        printf("║ Tipo:        CONSUMO NO LOCAL\n");
    }
    
    printf("║ Data:        %s\n", ped->data);
    printf("║ Status:      %s\n", ped->status);
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ ITENS DO PEDIDO:\n");
    printf("║\n");
    
    for (int i = 0; i < ped->total_itens; i++) {
        printf("║ • %dx %s\n", 
               ped->itens[i].quantidade, 
               ped->itens[i].item.nome);
        printf("║   R$ %.2f x %d = R$ %.2f\n", 
               ped->itens[i].item.preco,
               ped->itens[i].quantidade,
               ped->itens[i].item.preco * ped->itens[i].quantidade);
        printf("║\n");
    }
    
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ Subtotal:         R$ %.2f\n", ped->valor_total - ped->taxa_entrega);
    
    if (ped->eh_delivery) {
        printf("║ Taxa de Entrega:  R$ %.2f\n", ped->taxa_entrega);
    }
    
    printf("║ Valor Total:      R$ %.2f\n", ped->valor_total);
    printf("╚══════════════════════════════════════════════════╝\n");
}

void gravar_pedido(Pedido* ped) {
    FILE *arq = fopen(ARQUIVO_PEDIDOS, "ab");
    if (arq == NULL) {
        return;
    }

    fwrite(ped, sizeof(Pedido), 1, arq);
    fclose(arq);
}

void cadastrar_pedido() {
    Pedido* ped = (Pedido*) malloc(sizeof(Pedido));
    char continuar;
    int total_itens_solicitados = 0;
    ItemPedido todos_itens[100];
    char cpf_lido[20];
    Cliente cli;
    int achou = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              CADASTRAR NOVO PEDIDO               ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    ler_cpf_cliente(cpf_lido);

    FILE *arq_cli = fopen(ARQUIVO_CLIENTES, "rb");
    if (!arq_cli) {
        printf("Arquivo de clientes não encontrado!\n");
        free(ped);
        return;
    }

    

    while (!achou && fread(&cli, sizeof(Cliente), 1, arq_cli)) {
    if (strcmp(cli.cpf, cpf_lido) == 0) {
        achou = 1;
        printf("Cliente encontrado: %s\n", cli.nome);
        strcpy(ped->nome_cliente, cli.nome);
        strcpy(ped->telefone_cliente, cli.telefone);
        strcpy(ped->endereco_entrega, cli.endereco);
        }
    }
    
    fclose(arq_cli);

    if (strlen(cli.endereco) > 0) {
        ped->eh_delivery = 1;
        ped->taxa_entrega = TAXA_ENTREGA;
    } else {
        ped->eh_delivery = 0;
        ped->taxa_entrega = 0.0;
        strcpy(ped->endereco_entrega, "Consumo no local");
    }

    ler_data_pedido(ped->data);
    limpar_tela();
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║           ADICIONAR ITENS AO PEDIDO              ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    do {
        ResultadoBuscacardapio r = selecionar_produto_cardapio();
        
        if (!r.existe) {
            printf("\nNenhum item selecionado.\n");
            pausar();
            break;
        }

        printf("\n► Quantidade: ");
        int qtd;
        if (scanf("%d", &qtd) != 1 || qtd <= 0) {
            limparBuffer();
            printf("Quantidade inválida.\n");
            free(r.item);
            pausar();
            continue;
        }
        limparBuffer();

        todos_itens[total_itens_solicitados].item = *r.item;
        todos_itens[total_itens_solicitados].quantidade = qtd;
        total_itens_solicitados++;

        free(r.item);

        printf("\nItem adicionado!\n");
        printf("Total de itens no carrinho: %d\n", total_itens_solicitados);
        printf("\nAdicionar mais itens? (S/N): ");
        scanf(" %c", &continuar);
        limparBuffer();

    } while (continuar == 'S' || continuar == 's');

    if (total_itens_solicitados == 0) {
        printf("\nPedido cancelado: nenhum item adicionado.\n");
        free(ped);
        pausar();
        return;
    }

    int pedidos_criados = 0;
    int indice_item = 0;

    while (indice_item < total_itens_solicitados) {
        ped->numero_pedido = gerar_numero_pedido();
        ped->total_itens = 0;
        ped->valor_total = 0.0;

        for (int i = 0; i < MAX_ITENS_PEDIDO && indice_item < total_itens_solicitados; i++) {
            ped->itens[i] = todos_itens[indice_item];
            ped->valor_total += todos_itens[indice_item].item.preco * todos_itens[indice_item].quantidade;
            ped->total_itens++;
            indice_item++;
        }

        if (ped->eh_delivery) {
            ped->valor_total += ped->taxa_entrega;
        }

        strcpy(ped->status, "Em preparo");
        ped->ativo = 1;

        if (!confirma_dados_pedido(ped)) {
            printf("\nCadastro do pedido #%d cancelado pelo usuário.\n", ped->numero_pedido);
            free(ped);
            pausar();
            return;
        }

        gravar_pedido(ped);
        pedidos_criados++;
        
        printf("\n✓ Pedido #%d cadastrado com sucesso!\n", ped->numero_pedido);

        if (indice_item < total_itens_solicitados) {
            printf("\n⚠ Limite de %d itens por pedido atingido!\n", MAX_ITENS_PEDIDO);
            printf("Criando pedido adicional para os itens restantes...\n");
            pausar();
        }
    }

    if (pedidos_criados > 1) {
        printf("\n╔══════════════════════════════════════════════════╗\n");
        printf("║  ATENÇÃO: Foram criados %d pedidos               ║\n", pedidos_criados);
        printf("║  devido ao limite de %d itens por pedido.       ║\n", MAX_ITENS_PEDIDO);
        if (ped->eh_delivery) {
            printf("║  Taxa de entrega de R$ %.2f por pedido.        ║\n", TAXA_ENTREGA);
        }
        printf("╚══════════════════════════════════════════════════╝\n");
    }

    free(ped);
    pausar();
}

void listar_pedidos() {
    FILE *arq;
    Pedido ped;
    int contador = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                LISTA DE PEDIDOS                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            printf("Pedido #%d\n", ped.numero_pedido);
            printf("Cliente: %s\n", ped.nome_cliente);
            printf("Telefone: %s\n", ped.telefone_cliente);
            printf("Tipo: %s\n", ped.eh_delivery ? "DELIVERY" : "CONSUMO NO LOCAL");
            printf("Data: %s\n", ped.data);
            printf("Status: %s\n", ped.status);
            printf("Total de Itens: %d\n", ped.total_itens);
            printf("Valor: R$ %.2f\n", ped.valor_total);
            printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }

    fclose(arq);

    if (contador == 0) {
        printf("Nenhum pedido ativo encontrado.\n");
    } else {
        printf("Total de pedidos: %d\n", contador);
    }

    pausar();
}

void editar_pedido() {
    FILE *arq;
    Pedido ped;
    int numero, contador = 0;
    long pos_arquivo;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                 EDITAR PEDIDO                    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    printf("Pedidos cadastrados:\n\n");
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            printf(" %d - Pedido #%d - %s (R$ %.2f)\n",
                   contador, ped.numero_pedido, ped.nome_cliente, ped.valor_total);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum pedido ativo encontrado.\n");
        pausar();
        return;
    }

    printf("\nDigite o número do pedido: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq = fopen(ARQUIVO_PEDIDOS, "r+b");
    contador = 0;
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            if (contador == numero) {
                pos_arquivo = ftell(arq) - sizeof(Pedido);
                break;
            }
        }
    }

    printf("\nEditando Pedido #%d\n", ped.numero_pedido);
    printf("----------------------------------------------------\n");

    ler_nome_cliente(ped.nome_cliente);
    ler_telefone_cliente(ped.telefone_cliente);

    printf("► Status (Em preparo/Saiu para entrega/Entregue): ");
    ler_string(ped.status, sizeof(ped.status));

    fseek(arq, pos_arquivo, SEEK_SET);
    fwrite(&ped, sizeof(Pedido), 1, arq);
    fclose(arq);

    printf("\n Pedido atualizado com sucesso!\n");
    pausar();
}

void pesquisar_pedido() {
    FILE *arq;
    Pedido ped;
    int numero, contador = 0, encontrado = 0;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║               PESQUISAR PEDIDO                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    printf("Pedidos disponíveis:\n\n");
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            printf(" %d - Pedido #%d - %s\n", contador, ped.numero_pedido, ped.nome_cliente);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum pedido ativo encontrado.\n");
        pausar();
        return;
    }

    printf("\nDigite o número do pedido para ver detalhes: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    contador = 0;
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            if (contador == numero) {
                encontrado = 1;
            }
        }

        if (encontrado) {
            limpar_tela();
            exibir_pedido(&ped);
            encontrado = 2;
        }
    }

    if (encontrado == 0) {
        printf("\nPedido não encontrado.\n");
    }

    fclose(arq);
    pausar();
}

void excluir_pedido() {
    FILE *arq;
    Pedido ped;
    int numero, contador = 0;
    long pos_arquivo;
    char confirma;

    limpar_tela();
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                EXCLUIR PEDIDO                    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    arq = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arq == NULL) {
        printf("Nenhum pedido cadastrado ainda.\n");
        pausar();
        return;
    }

    printf("Pedidos cadastrados:\n\n");
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            printf(" %d - Pedido #%d - %s (R$ %.2f)\n",
                   contador, ped.numero_pedido, ped.nome_cliente, ped.valor_total);
        }
    }
    fclose(arq);

    if (contador == 0) {
        printf("\nNenhum pedido ativo encontrado.\n");
        pausar();
        return;
    }

    printf("\nDigite o número do pedido: ");
    scanf("%d", &numero);
    limparBuffer();

    if (numero < 1 || numero > contador) {
        printf("\nNúmero inválido!\n");
        pausar();
        return;
    }

    arq = fopen(ARQUIVO_PEDIDOS, "r+b");
    contador = 0;
    while (fread(&ped, sizeof(Pedido), 1, arq) == 1) {
        if (ped.ativo == 1) {
            contador++;
            if (contador == numero) {
                pos_arquivo = ftell(arq) - sizeof(Pedido);
                break;
            }
        }
    }

    printf("\nPedido: #%d - %s\n", ped.numero_pedido, ped.nome_cliente);
    printf("Valor: R$ %.2f\n\n", ped.valor_total);
    printf("Confirmar exclusão? (s/n): ");
    scanf(" %c", &confirma);
    limparBuffer();

    if (confirma == 's' || confirma == 'S') {
        ped.ativo = 0;
        fseek(arq, pos_arquivo, SEEK_SET);
        fwrite(&ped, sizeof(Pedido), 1, arq);
        printf("\n Pedido excluído com sucesso!\n");
    } else {
        printf("\nExclusão cancelada.\n");
    }

    fclose(arq);
    pausar();
}

void pedido() {
    int opcao;

    do {
        menu_pedidos();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrar_pedido();
                break;
            case 2:
                listar_pedidos();
                break;
            case 3:
                editar_pedido();
                break;
            case 4:
                pesquisar_pedido();
                break;
            case 5:
                excluir_pedido();
                break;
            case 0:
                printf("\nVoltando ao Menu Principal...\n");
                pausar();
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                pausar();
                break;
        }

    } while (opcao != 0);
}
