#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void limpar_tela() {
    system("clear||cls");
}



void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void pausar() {
    printf("\n Pressione Enter para continuar...");
    limparBuffer();
}