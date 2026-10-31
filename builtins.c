/*- Comandos Internos (Built-ins): Implementação obrigatória dos comandos executados no 
próprio processo DA minishell, sem recorrer a execvp() ou fork(). Estes comandos são implementados diretamente na shell e não requerem a criação de um novo processo para serem executados. Abaixo estão os comandos internos que devem ser implementados:
● cd <diretoria>: Altera a diretoria de trabalho atual da shell utilizando chdir(). 
● pwd: Mostra o caminho absoluto do diretório atual. 
● exit: Termina graciosamente a execução da minishell. 
● history: Apresenta a lista dos últimos comandos introduzidos. 
● echo: Apresenta no terminal o texto indicado como argumento. 
● help: Apresenta a lista de comandos embutidos e instruções de utilização.*/
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/history.h>

#define PATH_MAX 4096 // Define o tamanho máximo do caminho
// comando cd : altera a diretoria atual 
void cd (char **argv, int argc) {
    if (argc<2) {
    perror("Erro: argumento inválido para o comando cd");
    return;
    }
    // altera o diretorio e verifica se ocorreu algum erro
    if (chdir(argv[1]) == -1) {
        perror("Erro ao alterar a diretoria atual com chdir!");
    }
}

void pwd (char **argv, int argc) {
char *cwd = malloc(sizeof(char) *PATH_MAX); // aloca memória para armazenar o caminho atual
//verifica se o malloc deu erro 
if (cwd == NULL) {
    perror("Erro ao alocar memória para o caminho atual");
    return;
}
// obtém o diretório atual e verifica se ocorreu algum erro
if (getcwd(cwd, PATH_MAX) != NULL) {
    printf("%s\n", cwd);
} else {
    perror("Erro ao obter o diretório atual com getcwd!");
}
free(cwd);
}

void exit_shell (char **argv, int argc) {
    if (argc > 1) {
        perror("Erro: argumento inválido para o comando exit");
        return;
    }
    
    // termina a execução da minishell
    exit(0);
}

void history(char **argv, int argc) {
    if (argc > 1) {
        perror("Erro: argumento inválido para o comando history");
        return;
    }
  // Obtém o array com todo o histórico acumulado
    HIST_ENTRY **list = history_list();
    if (list == NULL) {
        printf("Nenhum histórico disponível.\n");
        return;
    }
    else if (list!=NULL) {
        for (int i = 0; list[i] != NULL; i++) {
        printf("linha %d: %s\n", i , list[i]->line);       
        }
    }
}

void echo(char **argv, int argc) {
    if (argc < 2) {
        perror("Erro: argumento inválido para o comando echo");
        return;
    }
    if (argv[1]== "-n") {
        for (int i = 2; i < argc; i++) {
            printf("%s ", argv[i]);
        }
    } else {
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
  
}


void help(char **argv, int argc) {
    if (argc > 1) {
        perror("Erro: argumento inválido para o comando help");
        return;
    }
    printf("Comandos internos disponíveis:\n");
    printf("cd <diretoria> : Altera a diretoria de trabalho atual da shell.\n");
    printf("pwd : Mostra o caminho absoluto do diretório atual.\n");
    printf("exit : Termina graciosamente a execução da minishell.\n");
    printf("history : Apresenta a lista dos últimos comandos introduzidos.\n");
    printf("echo <texto> : Apresenta no terminal o texto indicado como argumento.\n");
    printf("help : Apresenta a lista de comandos embutidos e instruções de utilização.\n");
}