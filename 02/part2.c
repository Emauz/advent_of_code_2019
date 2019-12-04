// run an intcode program and save the day!
// input: <comma separated input> <target value>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int runProgram(int program[]){
    for(int i=0; program[i] != 99; i+= 4){
        int calculated;
        if(program[i] == 1)
            calculated = program[program[i+1]] + program[program[i+2]];
        else
            calculated = program[program[i+1]] * program[program[i+2]];
        program[program[i+3]] = calculated;
    }
    return program[0];
}

int main(int argc, char *argv[]){
    int target = atoi(argv[2]);
    // convert to an int array
    int program[256];
    char *token = strtok(argv[1], ",");
    for(int i=0; token != NULL; i++){
        program[i] = atoi(token);
        token = strtok(NULL, ",");
    }
    // brute force answer
    // inputs are between 0 and 99 inclusive
    int copiedProgram[256];
    int result = 0;
    for(int i=0; result != target && i<=99; i++){
        for(int j=0; result != target && j<=99; j++){
            memcpy(copiedProgram, program, sizeof(program));
            copiedProgram[1] = i;
            copiedProgram[2] = j;
            result = runProgram(copiedProgram);
            if(result == target){
                printf("noun: %d, verb: %d\n", i, j);
                printf("Answer: %d\n", ((100*i)+j));
                return 0;
            }
        }
    }
    puts("No solution found");
    return 1;
}

