// Advent of code 2019
// Day 7 part 2
// Eric Moss

// calculate the best input parameters to a series of amplifiers 
// this time with a feedback loop
// input: <intcode file>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "intcode.h"

#define SERIES_LENGTH 5
#define BUFFER_SIZE 256
#define MIN_PHASE_SETTING 5
#define MAX_PHASE_SETTING 9

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
    bool foundA = false;
    bool foundB = false;
    bool foundC = false;
    bool foundD = false;
    bool foundE = false;
    for(int i=SERIES_LENGTH-1; i>=0; i--){
        switch(sequence % 10){
            case MIN_PHASE_SETTING+0:
                foundA = true;
                break;
            case MIN_PHASE_SETTING+1:
                foundB = true;
                break;
            case MIN_PHASE_SETTING+2:
                foundC = true;
                break;
            case MIN_PHASE_SETTING+3:
                foundD = true;
                break;
            case MIN_PHASE_SETTING+4:
                foundE = true;
                break;
        }
        sequence /= 10;
    }
    return foundA && foundB && foundC && foundD && foundE;
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
    
    Process *processes[SERIES_LENGTH];
    // initialize process control blocks for each computer
    for(int i=0; i < SERIES_LENGTH; i++){
        processes[i] = malloc(sizeof(Process));
        processes[i]->pc = 0;
        processes[i]->terminated = false;
        processes[i]->program = malloc(sizeof(int)*MAX_PROGRAM_LENGTH);
        memcpy(processes[i]->program, program, sizeof(int)*MAX_PROGRAM_LENGTH);
    }

    // first input is 0
    int runs = 0;
    int signal = 0;;
    while(!processes[SERIES_LENGTH-1]->terminated){
        FILE *input = tmpfile();
        FILE *output;
        prependInt(signal, input);
        for(int i=0; i<SERIES_LENGTH; i++){
            output = tmpfile();
            if(runs == 0)
                prependInt(splitSequence[i], input);
            runProgram(processes[i], input, output);
            fclose(input);
            input = freopen(NULL, "r", output);
        }
        runs++;
        fscanf(input, "%d", &signal);
        fclose(input);
    }
    // free processes table
    for(int i=0; i < SERIES_LENGTH; i++){
        free(processes[i]->program);
        free(processes[i]);
    }
    return signal;
}

int main(int argc, char *argv[]){
    FILE *openedFile = fopen(argv[1], "r");
    int *program = parseFile(openedFile);
    fclose(openedFile);
    int maxSignal = 0;
    int maxSetting = 0;
    for(int a=MIN_PHASE_SETTING*10000;a<=MAX_PHASE_SETTING*10000;a+= 10000){
        for(int b=MIN_PHASE_SETTING*1000;b<=MAX_PHASE_SETTING*1000;b+= 1000){
            for(int c=MIN_PHASE_SETTING*100;c<=MAX_PHASE_SETTING*100;c+= 100){
                for(int d=MIN_PHASE_SETTING*10;d<=MAX_PHASE_SETTING*10;d+= 10){
                    for(int e=MIN_PHASE_SETTING*1;e<=MAX_PHASE_SETTING*1;e+= 1){
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
