// Advent of code 2019
// Day 7 part 1
// Eric Moss

// calculate the best input parameters to a series of amplifiers 
// input: <intcode file>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "intcode.h"

#define SERIES_LENGTH 5
#define MAX_SEQUENCE 44444
#define BUFFER_SIZE 256

void prependInt(int value, FILE *openedFile){
    char currentContents[BUFFER_SIZE];

    openedFile = freopen(NULL, "r", openedFile);
    fgets(currentContents, BUFFER_SIZE, openedFile);
    openedFile = freopen(NULL, "w", openedFile);
    fprintf(openedFile, "%d %s", value, currentContents);
    openedFile = freopen(NULL, "r", openedFile);
}

// checks if sequence contains all phase settings
bool isValidSequence(int sequence){
    bool found0 = false;
    bool found1 = false;
    bool found2 = false;
    bool found3 = false;
    bool found4 = false;
    for(int i=SERIES_LENGTH-1; i>=0; i--){
        switch(sequence % 10){
            case 0:
                found0 = true;
                break;
            case 1:
                found1 = true;
                break;
            case 2:
                found2 = true;
                break;
            case 3:
                found3 = true;
                break;
            case 4:
                found4 = true;
                break;
        }
        sequence /= 10;
    }
    return found0 && found1 && found2 && found3 && found4;
}

int calculateSignal(int *program, int sequence){
    if(!isValidSequence(sequence))
        return -1;
    // splits base 10 integer into an array of individual digits
    int splitSequence[SERIES_LENGTH];
    for(int i=1; i <= SERIES_LENGTH; i++){
        splitSequence[SERIES_LENGTH - i] = sequence % 10;
        sequence -= (sequence % 10);
        sequence /= 10;
    }
    
    int *copiedProgram = malloc(sizeof(int) * MAX_PROGRAM_LENGTH);
    FILE *input = tmpfile();
    FILE *output = tmpfile();

    memcpy(copiedProgram, program, sizeof(int) * MAX_PROGRAM_LENGTH);
    prependInt(0, input);
    prependInt(splitSequence[0], input);
    runProgram(copiedProgram, input, output);
    // run program linked together
    for(int i=1; i<SERIES_LENGTH-1; i++){
        input = freopen(NULL, "r", output);
        prependInt(splitSequence[i], input);
        output = tmpfile();
        memcpy(copiedProgram, program, sizeof(int) * MAX_PROGRAM_LENGTH);
        runProgram(copiedProgram, input, output);
    }
    input = freopen(NULL, "r", output);
    prependInt(splitSequence[SERIES_LENGTH-1], input);
    output = tmpfile();

    memcpy(copiedProgram, program, sizeof(int) * MAX_PROGRAM_LENGTH);
    runProgram(copiedProgram, input, output);
    // get the result and return as integer
    output = freopen(NULL, "r", output);
    int signal;
    fscanf(output, "%d", &signal);
    return signal;
}

int main(int argc, char *argv[]){
    FILE *openedFile = fopen(argv[1], "r");
    int *program = parseFile(openedFile);
    fclose(openedFile);
    int maxSignal = 0;
    int maxSetting = 0;
    for(int a=0; a<=40000; a+= 10000){
        for(int b=0; b<=4000; b+= 1000){
            for(int c=0; c<=400; c+= 100){
                for(int d=0; d<=40; d+= 10){
                    for(int e=0; e<=4; e+= 1){
                        int i = a+b+c+d+e;
                        int signal = calculateSignal(program, i);
                        if(signal > maxSignal){
                            maxSignal = signal;
                            maxSetting = i;
                        }
                    }
                }
            }
        }
    }
    printf("Max signal: %d\n", maxSignal);
    printf("Max setting: %d\n", maxSetting);
}
