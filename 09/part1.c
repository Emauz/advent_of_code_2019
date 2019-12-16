// Eric Moss
// Advent of code 2019
// Day 9 part 1

// input: <file containing input>

// runs a complete intcode computer through the BOOST program

#include <stdio.h>
#include "intcode.h"

int main(int argc, char *argv[]){
    FILE *openedFile = fopen(argv[1], "r");
    Process *boost = createProcess(parseFile(openedFile));
    //fclose(openedFile);
    runProgram(boost, stdin, stdout);
}
