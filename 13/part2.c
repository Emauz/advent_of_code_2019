// Eric Moss
// Advent of code 2019
// Day 13 part 2

// beat an intcode powered game cabinet

// input: <File containing intcode program>

#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "intcode.h"

#define NUM_TILES 5
#define TILE_INFO 3
#define MAX_BLOCKS 10240
#define MAX_COLS 45
#define MAX_ROWS 30
#define DEBUG_ROW 45

char TILES[NUM_TILES] = {' ','#','@','P','*'};
int bounces[MAX_BLOCKS];
int currentPaddleLocation = 0;
int currentBounce = 0;
int currentBallX;
int currentBallY;
int mostRecentBallTarget;
int score = 0;
//int rows = 24;
int rows;

int calculateDirection(){
    if(bounces[currentBounce] == -1)
        return 0;

    if(bounces[currentBounce] > currentPaddleLocation)
        return 1;
    else if(bounces[currentBounce] < currentPaddleLocation)
        return -1;
    else 
        return 0;
}

// returns tile found
char writeNextTile(Process *process, FILE *in, FILE *out){
    //refresh();
    //usleep(10);
    long buf[TILE_INFO];
    for(int i=0; i<TILE_INFO; i++){
        out = freopen(NULL, "w", out);
        runProgram(process, in, out);
        if(process->inputFlag){
            in = freopen(NULL, "w", in);
            fprintf(in, "%d", calculateDirection());
            in = freopen(NULL, "r", in);
            runProgram(process, in, out);
            if(currentBallY == rows-2){
                currentBounce++;
            }
        }
        out = freopen(NULL, "r", out);
        fscanf(out, "%ld", &buf[i]);
    }
    mvprintw(buf[1]+1, buf[0]+1, "%c", TILES[buf[2]]);
    if(buf[1] > rows)
        rows = buf[1];
    if(buf[2] == 3)
        currentPaddleLocation = buf[0];
    if(TILES[buf[2]] == '*'){
        currentBallX = buf[0];
        currentBallY = buf[1];
        if(currentBallY == rows-2)
            mostRecentBallTarget = currentBallX;
    }
    // update debug information
    if(buf[0] == -1){
        score = buf[2];
    }
    char debug[BUFSIZ];
    sprintf(debug, "currentPaddleLocation: %d", currentPaddleLocation);
    mvaddstr(DEBUG_ROW, 0, debug); 
    sprintf(debug, "currentBounce: %d", currentBounce);
    mvaddstr(DEBUG_ROW+1, 0, debug); 
    sprintf(debug, "rows: %d", rows);
    mvaddstr(DEBUG_ROW+2, 0, debug); 
    sprintf(debug, "Ball at x:%d, y:%d", currentBallX, currentBallY);
    mvaddstr(DEBUG_ROW+3, 0, debug); 
    sprintf(debug, "Most recent ball target:%d", mostRecentBallTarget );
    mvaddstr(DEBUG_ROW+4, 0, debug); 
    sprintf(debug, "SCORE :%d", score);
    mvaddstr(DEBUG_ROW+6, 0, debug); 
    return TILES[buf[2]];
}

int main(int argc, char *argv[]){
    // read intcode file
    FILE *openedFile = fopen(argv[1], "r");
    long *program = parseFile(openedFile);
    program[0] = 2;
    Process *process = createProcess(program);
    fclose(openedFile);
    process->stopAtOutput = true;
    process->stopAtInput = true;

    //FILE *input = fopen("/dev/null", "r");
    //FILE *input = stdin;
    FILE *input = tmpfile();
    FILE *output = tmpfile();
    /*
    while(!process->terminated){
        printf("outputs: %d\n", count++);
        wrunProgram(process, input, output);
    }
    */

    // initialize ncurses screen
    initscr();
    noecho();
    int h, w;
    getmaxyx(stdscr, h, w);
    //WINDOW *win = newwin(h, w, 0, 0);
    for(int i=0; i<MAX_COLS; i++){
        mvaddch(0, i, (i%10)+48);
    }
    for(int i=0; i<MAX_ROWS; i++){
        mvaddch(i, 0, (i%10)+48);
    }
    // write board state to screen
    //Process *backupProcess = copyProcess(process);
    while(true){
        while(!process->terminated){
            writeNextTile(process, input, output);
            //getch();
        }
        refresh();
        bounces[currentBounce-1] = mostRecentBallTarget;
        //getch();
        // create fresh process
        free(process->program);
        free(process);
        process = createProcess(program);
        currentBounce = 0;
    }
    refresh();
    getch();
    clear();
    endwin();
    //printf("Block pieces found: %d\n", count);
}
