#include <string.h>
#include <unistd.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

#define PATH_MAX 4096 // Define o tamanho máximo do caminho

int is_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) {
        return 0; // Não é um comando built-in
    }
    // Verifica se o comando é um dos built-ins
    if (strcmp(cmd->name, "cd") == 0 ||
        strcmp(cmd->name, "pwd") == 0 ||
        strcmp(cmd->name, "exit") == 0 ||
        strcmp(cmd->name, "history") == 0 ||
        strcmp(cmd->name, "echo") == 0 ||
        strcmp(cmd->name, "help") == 0) {
        return 1; // É um comando built-in
    }
    return 0; // Não é um comando built-in
}

// comando cd : altera a diretoria atual 
void cd(char **argv) {
    if (argv[1] == NULL) {
        perror("Erro: argumento inválido para o comando cd");
        return;
    }
    // altera o diretório e verifica se ocorreu algum erro
    if (chdir(argv[1]) == -1) {
        perror("Erro ao alterar a diretoria atual com chdir!");
    }
}

void pwd(char **argv) {

    char *cwd = malloc(sizeof(char) * PATH_MAX); // Aloca memória para armazenar o caminho atual
    if (cwd == NULL) {
        perror("Erro ao alocar memória para o caminho atual");
        return;
    }
    if (getcwd(cwd, PATH_MAX) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("Erro ao obter o diretório atual com getcwd!");
    }
    free(cwd);
}

void echo(char **argv) {
    for (int i = 1; argv[i] != NULL; i++) {
        printf("%s", argv[i]);
        if (argv[i + 1] != NULL) {
            printf(" ");
        }
    }
    printf("\n");
}

void exit_shell(char **argv) {

    exit(0);
}

void history(char **argv) {
    HIST_ENTRY **list = history_list();
    if (list == NULL) {
        return;
    }

    for (int i = 0; list[i] != NULL; i++) {
        printf("%4d  %s\n", i + history_base, list[i]->line);
    } 
}

void help(char **argv) {
    
    printf("Comandos disponíveis:\n");
    printf("cd, pwd, exit, history, echo, help\n");
}

int execute_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) {
        return 0;
    }

    if (strcmp(cmd->name, "cd") == 0) {
        cd(cmd->argv);
        return 1;
    }
    if (strcmp(cmd->name, "pwd") == 0) {
        pwd(cmd->argv);
        return 1;
    }
    if (strcmp(cmd->name, "exit") == 0) {
        exit_shell(cmd->argv);
        return 1;
    }
    if (strcmp(cmd->name, "echo") == 0) {
        echo(cmd->argv);
        return 1;
    }
    if (strcmp(cmd->name, "history") == 0) {
        history(cmd->argv);
        return 1;
    }
    if (strcmp(cmd->name, "help") == 0) {
        help(cmd->argv);
        return 1;
    }

    return 0;
}