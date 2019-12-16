// Eric Moss
// Advent of code 2019
// Intcode computer header

#ifndef INTCODE_H
#define INTCODE_H

#define MAX_PROGRAM_LENGTH 1024

#include <stdio.h>
#include <stdbool.h>

typedef struct Process{
    int *program;
    int pc;
    bool terminated;
} Process;

void runProgram(Process *process, FILE *input, FILE *output);
int *parseFile(FILE *openedFile);

#endif
