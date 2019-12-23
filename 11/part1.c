// Eric Moss
// Advent of code 2019
// Day 11 part 1

// emergency hull painting robot
// determine number of spaces are painted over

// input: <file containing input>

#define HULL_HEIGHT 1000
#define HULL_WIDTH 1000
#define START_ROW 500
#define START_COL 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intcode.h"

typedef struct{
    int row;
    int col;
    int heading; // heading goes clockwise
}Painter;

int paint(char painting[HULL_HEIGHT][HULL_WIDTH], Painter *painter, 
    int color, int turn){

    painting[painter->row][painter->col] = color;
    if(turn == 0){
        painter->heading--;
    }
    else{
        painter->heading++;
    }
    switch(painter->heading%4){
        case 0:
            painter->row--;
            break;
        case 1:
            painter->col++;
            break;
        case 2:
            painter->row++;
            break;
        case 3:
            painter->col--;
            break;
        default:
            printf("HEADING % 4: %d\n", (painter->heading%4));
            break;
    }
    return painting[painter->row][painter->col];
}

Painter *createPainter(){
    Painter *output = malloc(sizeof(Painter));
    output->row = START_ROW;
    output->col = START_COL;
    output->heading = 400;
    return output;
}

void printHull(char painting[HULL_HEIGHT][HULL_WIDTH], Painter *painter){
    for(int i=0; i<HULL_HEIGHT; i++){
        for(int j=0; j<HULL_WIDTH; j++){
            if(painter->row == i && painter->col == j)
                switch(painter->heading % 4){
                    case 0:
                        putc('^', stdout);
                        break;
                    case 1:
                        putc('>', stdout);
                        break;
                    case 2:
                        putc('v', stdout);
                        break;
                    case 3:
                        putc('<', stdout);
                        break;
                }
            else{
                switch(painting[i][j]){
                    case -1:
                        putc(' ', stdout);
                        break;
                    case 0:
                        putc('B', stdout);
                        break;
                    case 1:
                        putc('W', stdout);
                        break;
                    default:
                        putc('?', stdout);
                        break;
                }
            }
        }
        puts("");
    }
}

int main(int argc, char *argv[]){
    FILE *openedFile = fopen(argv[1], "r");
    Process *process = createProcess(parseFile(openedFile));
    fclose(openedFile);
    process->stopAtOutput = true;

    char painting[HULL_HEIGHT][HULL_WIDTH];
    memset(painting, -1, HULL_HEIGHT * HULL_WIDTH);

    //FILE *progIn = stdin;
    FILE *progIn = tmpfile();
    FILE *progOut = tmpfile();
    int color, turn;
    int currentColor = 0;
    int count = 0;
    Painter *painter = createPainter();
    
    while(!process->terminated){
        //printHull(painting, painter);
        progIn = freopen(NULL, "w", progIn);
        fprintf(progIn, "%d", currentColor);
        progIn = freopen(NULL, "r", progIn);

        progOut = freopen(NULL, "w", progOut);
        runProgram(process, progIn, progOut);
        runProgram(process, progIn, progOut);
        progOut = freopen(NULL, "r", progOut);
        fscanf(progOut, "%d", &color);
        fscanf(progOut, "%d", &turn);

        currentColor = paint(painting, painter, color, turn);
        if(currentColor == -1){
            currentColor = 0;
            count++;
        }
    }
    printf("count: %d\n", count);
}
