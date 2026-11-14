#define _XOPEN_SOURCE 700

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handler (int sig ) {
    while (waitpid(-1, NULL, WNOHANG) > 0); // Recolhe o filho que terminou
    printf("Filho terminado (SIGCHLD recebido)\n");
   
}

// executa um comando simples, em primeiro ou em segundo plano ('&')
void execute_command(void) {
    pid_t pid;
    int background = 0;
    int i = 0;

    if (command_list == NULL || command_list->name == NULL) {
        return; // nenhum comando para executar
    }

    // conta o numero de argumentos do comando
    while (command_list->argv[i] != NULL) {
        i++;
    }

    // se o ultimo argumento for '&', o comando corre em segundo plano
    if (i > 1 && strcmp(command_list->argv[i - 1], "&") == 0) {
        background = 1;
        free(command_list->argv[i - 1]); // remove o '&' dos argumentos
        command_list->argv[i - 1] = NULL;
    }

    pid = fork();
    if (pid < 0) {
        perror("Erro ao criar processo filho com fork!");
        return;
    }

    // processo filho
    if (pid == 0) {
        signal(SIGINT, SIG_DFL); // o filho volta a aceitar ctrl + c e ctrl + z
        signal(SIGTSTP, SIG_DFL);
        execvp(command_list->name, command_list->argv);
        perror("Erro ao executar o comando com execvp!");
        exit(EXIT_FAILURE);
    }

    // processo pai
    if (background) {
        // nao espera pelo filho
        printf("Comando '%s' executado em segundo plano (PID: %d)\n", command_list->name, pid);
        signal(SIGCHLD, handler); // Vai executar a rotina de atendimento programada
        signal(SIGINT, SIG_IGN); // Ignora o ctrl + c
        signal(SIGTSTP, SIG_IGN); // Ignora o ctrl + z
        return;
    }

    // espera que o filho termine
    // (ECHILD acontece quando o SIGCHLD esta a ser ignorado: o filho ja foi recolhido)
    if (waitpid(pid, NULL, 0) == -1 && errno != ECHILD) {
        perror("Erro na recolha do filho com waitpid!");
    }
}