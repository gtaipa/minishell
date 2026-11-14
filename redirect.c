#define _XOPEN_SOURCE 700

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// aplica os redirecionamentos <, >, >> e 2> ao comando
void apply_redirections(CMD *cmd) {
    int fp;

    // < : le a entrada a partir de um ficheiro
    if (cmd->infile != NULL) {
        fp = open(cmd->infile, O_RDONLY);
        if (fp == -1) {
            perror("Erro ao redirecionar a entrada!");
            exit(1);
        }
        dup2(fp, 0);
        close(fp);
    }

    // > e >> : escreve a saida num ficheiro
    if (cmd->outfile != NULL) {
        char *file = cmd->outfile;
        int flags = O_CREAT | O_TRUNC | O_WRONLY;

        // o parser guarda ">>" como outfile = ">ficheiro" ou ">"
        if (file[0] == '>') {
            flags = O_CREAT | O_APPEND | O_WRONLY;
            file++;
            // caso "cmd >> ficheiro": o ficheiro ficou no ultimo argumento
            if (file[0] == '\0') {
                int i = 0;
                while (cmd->argv[i + 1] != NULL) {
                    i++;
                }
                file = cmd->argv[i];
                cmd->argv[i] = NULL;
            }
        }
        fp = open(file, flags, 0644);
        if (fp == -1) {
            perror("Erro ao redirecionar a saida!");
            exit(1);
        }
        dup2(fp, 1);
        close(fp);
    }

    // 2> : escreve os erros num ficheiro
    if (cmd->errfile != NULL) {
        fp = open(cmd->errfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fp == -1) {
            perror("Erro ao redirecionar a saida de erro!");
            exit(1);
        }
        dup2(fp, 2);
        close(fp);
    }
}