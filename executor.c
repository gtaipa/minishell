#define _XOPEN_SOURCE 700

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
// executa os comandos armazenados na lista encadeada, lidando com redirecionamentos e pipelines
void execute_command(void) {
 pid_t pid;
 if (command_list == NULL ||  command_list->name == NULL) {
 return ; // Nenhum comando para executar

 }  
 // vai buscar o pid atual 
pid = getpid(); 
int i =0;
// conta o número de argumentos do comando atual
while (command_list->argv[i] != NULL) {
    i++;
}
// se o último argumento for '&', significa que o comando deve ser executado em segundo plano
if (i > 0 && strcmp(command_list->argv[i - 1], "&") == 0) {
  
    // Se o último argumento for '&', significa que o comando deve ser executado em segundo plano
    // Remove o '&' da lista de argumentos
    command_list->argv[i - 1] = NULL; // Remove o '&'
    // Cria um novo processo filho para executar o comando em segundo plano
    pid = fork();
    if (pid < 0) {
        perror("Erro ao criar processo filho com fork!");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Processo filho
        // Executa o comando usando execvp e verifica se ocorreu algum erro
        if (execvp(command_list->name, command_list->argv) == -1) {
            perror("Erro ao executar o comando com execvp!");
            exit(EXIT_FAILURE); 
        }
    } else {
        // Processo pai
        // Não espera pelo filho,  imprime uma mensagem a dizer o que o comando foi executado em segundo plano
        printf("Comando '%s' executado em segundo plano (PID: %d)\n", command_list->name, pid);
       signal(SIGCHLD,SIG_IGN);// Ignora o sinal SIGCHLD para evitar processos zumbis
        return; // Retorna imediatamente sem esperar pelo filho
    }


}
pid=fork(); // cria um novo processo filho
if (pid < 0) {
    perror("Erro ao criar processo filho com fork!");
}
// processo filho
else if (pid ==0){

 if (execvp(command_list->name, command_list->argv) == -1) {
        perror("Erro ao executar o comando com execvp!");
        exit(EXIT_FAILURE); 
    }
}

// processo pai 
else {
// fica a espera que o filho termine 

int status;
// espera pelo processo filho e verifica se ocorreu algum erro
if (waitpid(pid, &status, 0) == -1) {
        perror("Erro na recolha do filho \n");
        exit(EXIT_FAILURE);
    }

    
}



}