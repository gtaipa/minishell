#include "header.h"

// decide quem executa a linha introduzida
void run_command(void) {
    if (command_list == NULL || command_list->name == NULL) {
        return; // nenhum comando para executar
    }

    // builtins executam no processo pai (apenas quando não há pipe)
    if (command_list->next == NULL && is_builtin(command_list)) {
        execute_builtin(command_list);
        return;
    }

    // comando simples, sem pipes nem redirecionamentos (inclui o '&')
    if (command_list->next == NULL && command_list->infile == NULL
        && command_list->outfile == NULL && command_list->errfile == NULL) {
        execute_command();
    }
    // tem pipes e/ou redirecionamentos
    else {
        execute_pipeline();
    }
}