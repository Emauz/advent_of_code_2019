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
    if(opcode == 99){
        parsedOpcode[0] = 99;
        return parsedOpcode;
    }
    parsedOpcode[0] = opcode % 100;
    opcode = (opcode - (opcode % 100)) / 100;
    for(int i=1; i<OPCODE_SIZE; i++){
        parsedOpcode[i] = opcode % 10;
        opcode = (opcode - (opcode % 10)) / 10;
    }
    return parsedOpcode;
}

// gets value from $target while program is in $mode
long getVal(Process *process, int target, int mode){
    long *program = process->program;
    int relativeBase = process->relativeBase;
    switch(mode){
        case 0:
            return program[program[target]];
        case 1:
            return program[target];
        case 2:
            return program[relativeBase + program[target]];
        default:
            puts("ERROR: UNHANDLED OPCODE");
            exit(1);
    }
}

void putVal(Process *process, int target, int mode, long value){
    long *program = process->program;
    int relativeBase = process->relativeBase;
    switch(mode){
        case 0:
            program[program[target]] = value;
            break;
        case 2:
            program[relativeBase + program[target]] = value;
            break;
        default:
            puts("ERROR: UNHANDLED OPCODE");
            exit(1);
    }
}

void runProgram(Process *process, FILE *input, FILE *output){
    //puts("program starting");
    int pc = process->pc;
    long *program = process->program;
    long val1, val2, val3, result;
    while(!process->terminated){
        int *opcode = parseOpcode(program[pc]);
        switch(opcode[0]){
            case 1:
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                result = val1 + val2;
                //program[program[pc+3]] = result;
                putVal(process, pc+3, opcode[3], result);
                pc += 4;
                break; 
            case 2:
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                result = val1 * val2;
                //program[program[pc+3]] = result;
                putVal(process, pc+3, opcode[3], result);
                pc += 4;
                break;
            case 3:
                fscanf(input, "%ld", &result);
                //printf("got: %ld\n", result);
                //program[program[pc+1]] = result;
                putVal(process, pc+1, opcode[1], result);
                val1 = getVal(process, pc+1, opcode[1]);
                pc += 2;
                break;
            case 4:
                val1 = getVal(process, pc+1, opcode[1]);
                fprintf(output, "%ld\n", val1);
                pc += 2;
                process->pc = pc;
                //return;
                break;
            case 5:
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                pc = (val1 != 0) ? val2 : pc + 3;
                break;
            case 6: 
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                pc = (val1 == 0) ? val2 : pc + 3;
                break;
            case 7:
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                result = (val1 < val2) ? 1 : 0;
                //program[program[pc+3]] = (val1 < val2) ? 1 : 0;
                putVal(process, pc+3, opcode[3], result);
                pc += 4;
                break;
            case 8:
                val1 = getVal(process, pc+1, opcode[1]);
                val2 = getVal(process, pc+2, opcode[2]);
                result = (val1 == val2) ? 1 : 0;
                //program[program[pc+3]] = (val1 == val2) ? 1 : 0;
                putVal(process, pc+3, opcode[3], result);
                pc += 4;
                break;
            case 9:
                val1 = getVal(process, pc+1, opcode[1]);
                process->relativeBase += val1;
                pc += 2;
                break;
            case 99:
                process->terminated = true;
                break;
            default:
                puts("Error: Invalid opcode");
                exit(1);
        }
        // update process block pc
        process->pc = pc;
    }
}

long *parseFile(FILE *openedFile){
    // parse program from comma-separated input file
    long *program = calloc(MAX_PROGRAM_LENGTH, sizeof(int));
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, openedFile);
    char *token = strtok(buffer, ",");
    for(int i=0; token != NULL; i++){
        program[i] = atol(token);
        token = strtok(NULL, ",");
    }
    return program;
}

Process *createProcess(long *program){
    Process *output = malloc(sizeof(Process));
    output->program = calloc(sizeof(long), MAX_PROGRAM_LENGTH);
    memcpy(output->program, program, sizeof(long) * MAX_PROGRAM_LENGTH);
    output->pc = 0;
    output->relativeBase = 0;
    output->terminated = false;
}

