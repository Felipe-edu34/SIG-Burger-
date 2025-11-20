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



void tela_inicial () {
    limpar_tela();
    func_Ani(tempo_animacao);
    printf("                        .*.                             @@@@........@@@@                             ╔══════                            \n");
    func_Ani(tempo_animacao);
    printf("                       .***.                        @@-.---------------..:@@                         ║                           \n");
    func_Ani(tempo_animacao);
    printf("                      .*****.                   @@.-------@-------@=------..=@                       ║                        \n");
    func_Ani(tempo_animacao);
    printf("                     :########:                @@.---@------------------------.@@                    ║                        \n");
    func_Ani(tempo_animacao);
    printf("                   ..**********+.             @---------------------------@-----.@                ========                    \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.         @@---------------@-------------------.@@         ===--------------====           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.         @@---------@---------------@---------@--@         ==================             \n");
    func_Ani(tempo_animacao); 
    printf("                  .**************.        @----------------------------------------@        |++++++++++++++++++|           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.       @-----------@-----------------------------:@       |++++++++++++++++++|          \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.      @@-----------------@----------------+-------@@      |++++++++++++++++++|         \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.      @@--@------@------@------@----------@-------@@      |++++++++++++++++++|           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.       @@@@@@@@@@@@@:@@::@@@@@@@@@@@@@@@@@@@@@@@@@@       |++++++++++++++++++|            \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.         +++++++++BEM VINDO AO SIG-BURGER+++++++++        |==================|           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.       @@@@@@.@@@@...@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       |==================|           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.      @.------------------------------------------.@      |++++++++++++++++++|           \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.      @--------------------------------------------@      |++++++++++++++++++|            \n");
    func_Ani(tempo_animacao);
    printf("                  .**************.       @==========================================@       |++++++++++++++++++|          \n");
    func_Ani(tempo_animacao);
    printf("                   .************-           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            ++++++++++++++++          \n");
    func_Ani(tempo_animacao);
    printf("\n");
    
}





