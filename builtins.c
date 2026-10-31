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
    char *cwd;

    (void)argv;
    (void)argc;

    cwd = malloc(sizeof(char) * PATH_MAX); // aloca memória para armazenar o caminho atual
    if (cwd == NULL) {
        perror("Erro ao alocar memória para o caminho atual");
        return;
    }

    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("Erro ao obter o caminho atual com getcwd");
        free(cwd);
        return;
    }

    printf("%s\n", cwd);
    free(cwd);
}
