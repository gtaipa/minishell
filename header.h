/*
 *  header.h
 *  parse
 *
 *  Created by Pedro Sobral on 06/10/26.
 *
 */
#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAXARGS 10

typedef struct command {
    char *name;
    char *argv[MAXARGS];
    char *infile;
    char *outfile;
    char *errfile;
    struct command *next;
} CMD;

extern CMD *command_list;


CMD *insert_command(void);
void free_command_list();
void print_command_list();
void parse_line(char *);
void execute_command(void);
#endif