#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cliente.h"

#define ARQUIVO_CLIENTES "dados/clientes.dat"

void menu_clientes(void) {
    limpar_tela();
    printf("\n");
    printf("        ╔══════════════════════════════════════════════════╗\n");
    printf("        ║         SIG-BURGER - GESTÃO DE CLIENTES          ║\n");
    printf("        ╠══════════════════════════════════════════════════╣\n");
    printf("        ║                                                  ║\n");
    printf("        ║ ► 1. Cadastrar Cliente                           ║\n");
    printf("        ║ ► 2. Listar Clientes                             ║\n");
    printf("        ║ ► 3. Editar Cliente                              ║\n");
    printf("        ║ ► 4. Excluir Cliente                             ║\n");
    printf("        ║ ► 0. Voltar ao Menu Principal                    ║\n");
    printf("        ║                                                  ║\n");
    printf("        ╚══════════════════════════════════════════════════╝\n");
    printf("\n        Escolha uma opção: ");
}

void cadastrar_cliente(void) {
    Cliente cli;
    FILE* fp;
    
    limpar_tela();
    printf("\n");
    printf("        ╔══════════════════════════════════════════════════╗\n");
    printf("        ║            CADASTRAR NOVO CLIENTE                ║\n");
    printf("        ╚══════════════════════════════════════════════════╝\n\n");
    
    printf("        Nome completo: ");
    ler_string(cli.nome, sizeof(cli.nome));
    
    printf("        Telefone: ");
    ler_string(cli.telefone, sizeof(cli.telefone));
    
    printf("        Endereço completo: ");
    ler_string(cli.endereco, sizeof(cli.endereco));
    
    cli.status = 1;
    
    fp = fopen(ARQUIVO_CLIENTES, "ab");
    if (fp == NULL) {
        printf("\n Erro ao abrir arquivo!\n");
        pausar();
        return;
    }
    
    fwrite(&cli, sizeof(Cliente), 1, fp);
    fclose(fp);
    
    printf("\n Cliente cadastrado com sucesso!\n");
    pausar();
}

void listar_clientes(void) {
    FILE* fp;
    Cliente cli;
    int contador = 0;
    
    limpar_tela();
    printf("\n");
    printf("        ╔══════════════════════════════════════════════════╗\n");
    printf("        ║              LISTA DE CLIENTES                   ║\n");
    printf("        ╚══════════════════════════════════════════════════╝\n\n");
    
    fp = fopen(ARQUIVO_CLIENTES, "rb");
    if (fp == NULL) {
        printf("        Nenhum cliente cadastrado ainda.\n");
        pausar();
        return;
    }
    
    printf("        ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
    
    while (fread(&cli, sizeof(Cliente), 1, fp) == 1) {
        if (cli.status == 1) {
            contador++;
            printf("        Cliente %d\n", contador);
            printf("        Nome: %s\n", cli.nome);
            printf("        Telefone: %s\n", cli.telefone);
            printf("        Endereço: %s\n", cli.endereco);
            printf("\n        ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }
    
    fclose(fp);
    
    if (contador == 0) {
        printf("        Nenhum cliente ativo encontrado.\n");
    } else {
        printf("        Total de clientes: %d\n", contador);
    }
    
    pausar();
}

void editar_cliente(void) {
    FILE* fp;
    Cliente cli;
    int numero, contador = 0, posicao = -1;
    long pos_arquivo;
    
    limpar_tela();
    printf("\n");
    printf("        ╔══════════════════════════════════════════════════╗\n");
    printf("        ║               EDITAR CLIENTE                     ║\n");
    printf("        ╚══════════════════════════════════════════════════╝\n\n");
    
    fp = fopen(ARQUIVO_CLIENTES, "rb");
    if (fp == NULL) {
        printf("        Nenhum cliente cadastrado ainda.\n");
        pausar();
        return;
    }
    
    printf("        Clientes cadastrados:\n\n");
    while (fread(&cli, sizeof(Cliente), 1, fp) == 1) {
        if (cli.status == 1) {
            contador++;
            printf("        %d - %s\n", contador, cli.nome);
        }
    }
    fclose(fp);
    
    if (contador == 0) {
        printf("\n        Nenhum cliente ativo encontrado.\n");
        pausar();
        return;
    }
    
    printf("\n        Digite o número do cliente: ");
    scanf("%d", &numero);
    limparBuffer();
    
    if (numero < 1 || numero > contador) {
        printf("\n        Número inválido!\n");
        pausar();
        return;
    }
    
    fp = fopen(ARQUIVO_CLIENTES, "r+b");
    contador = 0;
    while (fread(&cli, sizeof(Cliente), 1, fp) == 1) {
        if (cli.status == 1) {
            contador++;
            if (contador == numero) {
                posicao = contador;
                pos_arquivo = ftell(fp) - sizeof(Cliente);
                break;
            }
        }
    }
    
    if (posicao == -1) {
        printf("\n        Cliente não encontrado!\n");
        fclose(fp);
        pausar();
        return;
    }
    
    printf("\n        Dados atuais:\n");
    printf("        Nome: %s\n", cli.nome);
    printf("        Telefone: %s\n", cli.telefone);
    printf("        Endereço: %s\n", cli.endereco);
    
    printf("\n        Novos dados:\n\n");
    
    printf("        Novo nome: ");
    ler_string(cli.nome, sizeof(cli.nome));
    
    printf("        Novo telefone: ");
    ler_string(cli.telefone, sizeof(cli.telefone));
    
    printf("        Novo endereço: ");
    ler_string(cli.endereco, sizeof(cli.endereco));
    
    fseek(fp, pos_arquivo, SEEK_SET);
    fwrite(&cli, sizeof(Cliente), 1, fp);
    fclose(fp);
    
    printf("\n Cliente atualizado com sucesso!\n");
    pausar();
}

void excluir_cliente(void) {
    FILE* fp;
    Cliente cli;
    int numero, contador = 0, posicao = -1;
    long pos_arquivo;
    char confirma;
    
    limpar_tela();
    printf("\n");
    printf("        ╔══════════════════════════════════════════════════╗\n");
    printf("        ║              EXCLUIR CLIENTE                     ║\n");
    printf("        ╚══════════════════════════════════════════════════╝\n\n");
    
    fp = fopen(ARQUIVO_CLIENTES, "rb");
    if (fp == NULL) {
        printf("        Nenhum cliente cadastrado ainda.\n");
        pausar();
        return;
    }
    
    printf("        Clientes cadastrados:\n\n");
    while (fread(&cli, sizeof(Cliente), 1, fp) == 1) {
        if (cli.status == 1) {
            contador++;
            printf("        %d - %s\n", contador, cli.nome);
        }
    }
    fclose(fp);
    
    if (contador == 0) {
        printf("\n        Nenhum cliente ativo encontrado.\n");
        pausar();
        return;
    }
    
    printf("\n        Digite o número do cliente: ");
    scanf("%d", &numero);
    limparBuffer();
    
    if (numero < 1 || numero > contador) {
        printf("\n        Número inválido!\n");
        pausar();
        return;
    }
    
    fp = fopen(ARQUIVO_CLIENTES, "r+b");
    contador = 0;
    while (fread(&cli, sizeof(Cliente), 1, fp) == 1) {
        if (cli.status == 1) {
            contador++;
            if (contador == numero) {
                posicao = contador;
                pos_arquivo = ftell(fp) - sizeof(Cliente);
                break;
            }
        }
    }
    
    if (posicao == -1) {
        printf("\n        Cliente não encontrado!\n");
        fclose(fp);
        pausar();
        return;
    }
    
    printf("\n        Cliente: %s\n", cli.nome);
    printf("        Telefone: %s\n\n", cli.telefone);
    printf("        Confirmar exclusão? (s/n): ");
    scanf("%c", &confirma);
    limparBuffer();
    
    if (confirma == 's' || confirma == 'S') {
        cli.status = 0;
        fseek(fp, pos_arquivo, SEEK_SET);
        fwrite(&cli, sizeof(Cliente), 1, fp);
        printf("\n        Cliente excluído com sucesso!\n");
    } else {
        printf("\n        Exclusão cancelada!\n");
    }
    
    fclose(fp);
    pausar();
}

void cliente(void) {
    int opcao;
    
    do {
        menu_clientes();
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1:
                cadastrar_cliente();
                break;
            case 2:
                listar_clientes();
                break;
            case 3:
                editar_cliente();
                break;
            case 4:
                excluir_cliente();
                break;
            case 0:
                printf("\n Retornando ao menu principal...\n");
                pausar();
                break;
            default:
                printf("\n Opção inválida! Tente novamente.\n");
                pausar();
        }
    } while(opcao != 0);
}
