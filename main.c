/*
 *  main.c
 *  parse
 */
#define _XOPEN_SOURCE 700 // 
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>



int main(int argc, const char *argv[]) {

// 1. Variáveis para as regras dos sinais
    struct sigaction sa_chld, sa_ign;

    
    sa_chld.sa_handler = handler; // Aponta para o handler (com waitpid WNOHANG)
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART; // Evita que a shell vá abaixo se for interrompida
    sigaction(SIGCHLD, &sa_chld, NULL);

    
    sa_ign.sa_handler = SIG_IGN;  // Ação: Ignorar
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = 0;
    sigaction(SIGINT, &sa_ign, NULL);  // Aplica ao Ctrl+C
    sigaction(SIGTSTP, &sa_ign, NULL); // Aplica ao Ctrl+Z

    char *line;
    
    while (1) {
        line = readline("msh$ ");// 
        
        // Trata o sinal EOF (Ctrl+D)
        if (line == NULL) {
            printf("\nExiting...\n");
            break;
        }
        if (line[0] == '\0') { // Verifica se a linha está vazia
            free(line); // Liberta a memória alocada por readline
            continue; // Pula para a próxima iteração do loop
        }
        // Verifica se a linha digitada é "exit"
        if (strcmp(line, "exit") == 0) {
            free(line); // Libertação da memória de readline antes de sair
            break;
        }
        
        // Adiciona à história caso a linha não esteja vazia
        if (*line) {
            add_history(line);
        }

        
        parse_line(line);
        run_command();
        print_command_list();
        free_command_list();
        
        free(line); // Liberta a memória alocada por readline
    }
    
    return 0;
}
