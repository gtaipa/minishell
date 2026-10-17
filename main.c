/*
 *  main.c
 *  parse
 */
#define _XOPEN_SOURCE 700 // 
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, const char *argv[]) {
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
        print_command_list();
        free_command_list();
        
        free(line); // Liberta a memória alocada por readline
    }
    
    return 0;
}
