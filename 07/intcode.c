// Eric Moss
// Advent of code 2019
// Intcode computer

// Run an intcode computer through a given program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intcode.h"

#define BUFFER_SIZE 4096
#define OPCODE_SIZE 4

// returns a pointer to an int array of size OPCODE_SIZE
// array[0] is the opcode, array[i] is the mode for argument i
int *parseOpcode(int opcode){
    int *parsedOpcode = calloc(OPCODE_SIZE, sizeof(int));
    parsedOpcode[0] = opcode % 100;
    opcode = (opcode - (opcode % 100)) / 100;
    for(int i=1; i<OPCODE_SIZE; i++){
        parsedOpcode[i] = opcode % 10;
        opcode = (opcode - (opcode % 10)) / 10;
    }
    return parsedOpcode;
}

// gets value from $target while program is in $mode
int getVal(int program[], int target, int mode){
    switch(mode){
        case 0:
            return program[program[target]];
        case 1:
            return program[target];
    }
}

void runProgram(int *program, FILE *input, FILE *output){
    //puts("program starting");
    int pc = 0;
    int val1, val2, val3, result;
    while(program[pc] != 99){
        int *opcode = parseOpcode(program[pc]);
        switch(opcode[0]){
            case 1:
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                result = val1 + val2;
                program[program[pc+3]] = result;
                pc += 4;
                break; 
            case 2:
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                result = val1 * val2;
                program[program[pc+3]] = result;
                pc += 4;
                break;
            case 3:
                //printf("in: ");
                fscanf(input, "%d", &result);
                program[program[pc+1]] = result;
                pc += 2;
                break;
            case 4:
                val1 = getVal(program, pc+1, opcode[1]);
                fprintf(output, "%d\n", val1);
                pc += 2;
                break;
            case 5:
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                pc = (val1 != 0) ? val2 : pc + 3;
                break;
            case 6: 
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                pc = (val1 == 0) ? val2 : pc + 3;
                break;
            case 7:
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                program[program[pc+3]] = (val1 < val2) ? 1 : 0;
                pc += 4;
                break;
            case 8:
                val1 = getVal(program, pc+1, opcode[1]);
                val2 = getVal(program, pc+2, opcode[2]);
                program[program[pc+3]] = (val1 == val2) ? 1 : 0;
                pc += 4;
                break;
        }
    }
}

int *parseFile(FILE *openedFile){
    // parse program from comma-separated input file
    int *program = calloc(MAX_PROGRAM_LENGTH, sizeof(int));
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, openedFile);
    char *token = strtok(buffer, ",");
    for(int i=0; token != NULL; i++){
        program[i] = atoi(token);
        token = strtok(NULL, ",");
    }
    return program;
}

