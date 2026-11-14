#define _XOPEN_SOURCE 700

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// executa varios comandos ligados por pipes (cmd1 | cmd2 | ...)
void execute_pipeline(void) {
    CMD *cmd;
    int fds[2];
    int input = 0; // de onde o comando atual vai ler (0 = stdin)
    int n = 0;     // numero de filhos criados
    pid_t pid;

    for (cmd = command_list; cmd != NULL; cmd = cmd->next) {
        // se houver um comando a seguir, cria um pipe para lhe passar a saida
        if (cmd->next != NULL && pipe(fds) == -1) {
            perror("Erro ao criar o pipe!");
            break;
        }

        pid = fork();
        if (pid < 0) {
            perror("Erro ao criar processo filho com fork!");
            break;
        }

        // processo filho
        if (pid == 0) {
            // le do pipe do comando anterior
            if (input != 0) {
                dup2(input, 0);
                close(input);
            }
            // escreve para o pipe do proximo comando
            if (cmd->next != NULL) {
                dup2(fds[1], 1);
                close(fds[0]);
                close(fds[1]);
            }
            apply_redirections(cmd);
            execvp(cmd->name, cmd->argv);
            perror("Erro ao executar o comando com execvp!");
            exit(1);
        }

        // processo pai: fecha o que ja nao usa e guarda a leitura para o proximo
        n++;
        if (input != 0) {
            close(input);
        }
        if (cmd->next != NULL) {
            close(fds[1]);
            input = fds[0];
        }
    }

    // espera que todos os filhos terminem
    while (n > 0) {
        wait(NULL);
        n--;
    }
}