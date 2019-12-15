// Eric Moss
// Advent of code 2019
// Intcode computer header

#ifndef INTCODE_H
#define INTCODE_H

#define MAX_PROGRAM_LENGTH 1024

#include <stdio.h>

void runProgram(int *program, FILE *input, FILE *output);
int *parseFile(FILE *openedFile);

#endif
