#define _XOPEN_SOURCE 700

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// executa os comandos armazenados na lista encadeada, lidando com redirecionamentos e pipelines
void execute_command(void) {
 pid_t pid;
 if (command_list == NULL ||  command_list->name == NULL) {
 return ; // Nenhum comando para executar

 }  
 // vai buscar o pid atual 
pid = getpid(); 
pid=fork(); // cria um novo processo filho
if (pid < 0) {
    perror("Erro ao criar processo filho com fork!");
}
// processo filho
else if (pid ==0){
 execvp(command_list->name, command_list->argv); // executa o comando

 if (execvp(command_list->name, command_list->argv) == -1) {
        perror("Erro ao executar o comando com execvp!");
        exit(EXIT_FAILURE); 
    }
}

// processo pai 
else {
// fica a espera que o filho termine 

int status;
waitpid(pid, &status, 0); // espera o processo filho terminar

if (waitpid(pid, &status, 0) == -1) {
        perror("Erro na recolha do filho \n");
        exit(EXIT_FAILURE);
    }

    
}



}