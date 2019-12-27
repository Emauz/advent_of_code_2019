// Eric Moss
// Advent of code 2019
// Day 13 part 1

// run an intcode powered game cabinet

// input: <File containing intcode program>

#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include "intcode.h"

#define NUM_TILES 5
#define TILE_INFO 3

char TILES[NUM_TILES] = {' ','W','B','P','*'};

// returns tile found
char writeNextTile(WINDOW *win, Process *process, FILE *in, FILE *out){
    long buf[TILE_INFO];
    for(int i=0; i<TILE_INFO; i++){
        out = freopen(NULL, "w", out);
        runProgram(process, in, out);
        out = freopen(NULL, "r", out);
        fscanf(out, "%ld", &buf[i]);
    }
    mvprintw(buf[1], buf[0], "%c", TILES[buf[2]]);
    return TILES[buf[2]];
}

int main(int argc, char *argv[]){
    // read intcode file
    FILE *openedFile = fopen(argv[1], "r");
    Process *process = createProcess(parseFile(openedFile));
    fclose(openedFile);
    process->stopAtOutput = true;

    FILE *input = stdin;
    FILE *output = tmpfile();

    // initialize ncurses screen
    initscr();
    int h, w;
    getmaxyx(stdscr, h, w);
    WINDOW *win = newwin(h, w, 0, 0);
    // write board state to screen
    int count = 0;
    while(!process->terminated){
        if(writeNextTile(win, process, input, output) == 'B')
            count++;
    }
    getch();
    clear();
    endwin();
    printf("Block pieces found: %d\n", count);
}
