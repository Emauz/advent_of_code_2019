// Eric Moss
// Advent of code 2019
// Intcode computer header

#ifndef INTCODE_H
#define INTCODE_H

#define MAX_PROGRAM_LENGTH 64000

#include <stdio.h>
#include <stdbool.h>

typedef struct Process{
    long *program;
    int pc;
    int relativeBase;
    bool terminated;
} Process;

void runProgram(Process *process, FILE *input, FILE *output);
long *parseFile(FILE *openedFile);
Process *createProcess(long *program);

#endif
