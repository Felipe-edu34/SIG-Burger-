#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN64 // Caso windows 64, use:
    #include <windows.h>  //Trecho de código tirado do ChatGPT
#else // Se não, Linux,Mcos, use:
    #include <unistd.h>
#endif


int tempo_animacao = 100; // Tempo padrão para animação em milissegundos

void limpar_tela() {
    system("clear||cls");
}



void func_Ani(int tempo){  //Função retirada do ChatGPT 4.0
    #ifdef _WIN64
        Sleep(tempo);
    #else
        usleep(tempo * 1000); 
    #endif
}



void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void pausar() {
    printf("\n Pressione Enter para continuar...");
    limparBuffer();
}



char* ler_string(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        int len = strcspn(destino, "\n"); 

        if (destino[len] == '\n') {
            destino[len] = '\0';
        } else {
            limparBuffer();      
        }
        return destino;
    }
    return NULL; 
}



char remover_acento(char c) {
    unsigned char uc = (unsigned char)c;

    switch (uc) {
        case 0xC1: case 0xE1: return 'a'; // á Á
        case 0xC2: case 0xE2: return 'a'; // â Â
        case 0xC0: case 0xE0: return 'a'; // à À
        case 0xC3: case 0xE3: return 'a'; // ã Ã

        case 0xC9: case 0xE9: return 'e'; // é É
        case 0xCA: case 0xEA: return 'e'; // ê Ê

        case 0xCD: case 0xED: return 'i'; // í Í

        case 0xD3: case 0xF3: return 'o'; // ó Ó
        case 0xD4: case 0xF4: return 'o'; // ô Ô
        case 0xD5: case 0xF5: return 'o'; // õ Õ

        case 0xDA: case 0xFA: return 'u'; // ú Ú

        case 0xC7: case 0xE7: return 'c'; // ç Ç
    }

    return c;
}



int strcmp_sem_acento(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = remover_acento(tolower((unsigned char)*s1));
        char c2 = remover_acento(tolower((unsigned char)*s2));

        if (c1 != c2)
            return (unsigned char)c1 - (unsigned char)c2;

        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}





void tela_inicial () {
    limpar_tela();
    printf("\n\n\n\n\n\n\n");
    func_Ani(tempo_animacao);
    printf("\t\t                         .*.                            @@@@........@@@@                             ╔══════          \n");
    func_Ani(tempo_animacao);
    printf("\t\t                        .***.                       @@-.---------------..:@@                         ║                \n");
    func_Ani(tempo_animacao);
    printf("\t\t                       .*****.                  @@.-------@-------@=------..=@                       ║                \n");
    func_Ani(tempo_animacao);
    printf("\t\t                    :##########:               @@.---@------------------------.@@                    ║                \n");
    func_Ani(tempo_animacao);
    printf("\t\t                   .************.             @---------------------------@-----.@                ========            \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.         @@---------------@-------------------.@@         ===--------------====     \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.         @@---------@---------------@---------@--@         ==================       \n");
    func_Ani(tempo_animacao); 
    printf("\t\t                  .**************.        @----------------------------------------@        |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.       @-----------@-----------------------------:@       |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.      @@-----------------@----------------+-------@@      |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.      @@--@------@------@------@----------@-------@@      |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.       @@@@@@@@@@@@@:@@::@@@@@@@@@@@@@@@@@@@@@@@@@@       |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.         +++++++++BEM VINDO AO SIG-BURGER+++++++++        |==================|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.       @@@@@@.@@@@...@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       |==================|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.      @.------------------------------------------.@      |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.      @--------------------------------------------@      |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                  .**************.       @==========================================@       |++++++++++++++++++|      \n");
    func_Ani(tempo_animacao);
    printf("\t\t                   .************-           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            ++++++++++++++++        \n");
    func_Ani(tempo_animacao);
    printf("\n");
    
}





