// run an intcode program and save the day!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    // convert to an int array
    int program[256];
    char *token = strtok(argv[1], ",");
    for(int i=0; token != NULL; i++){
        program[i] = atoi(token);
        token = strtok(NULL, ",");
    }
    // init position 1 and 2 to prescribed values
    program[1] = 12;
    program[2] = 2;
    // run program
    for(int i=0; program[i] != 99; i+= 4){
        int calculated;
        if(program[i] == 1)
            calculated = program[program[i+1]] + program[program[i+2]];
        else
            calculated = program[program[i+1]] * program[program[i+2]];
        program[program[i+3]] = calculated;
    }
    printf("Position 0: %d\n", program[0]);
    return 0;
}
