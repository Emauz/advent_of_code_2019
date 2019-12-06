// Find shortest intersection between two wires
// input: <first wire spec> <second wire spec>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PANEL_SIZE 30000

int panel[PANEL_SIZE][PANEL_SIZE];
int row;
int col;;
int minDistance = 30000;
int totalDistance = 0;

// Draw a single segment of wire on the panel
void drawSegment(char *segment, int representation, char pipe){
    // determine direction
    char row_offset = 0;
    char col_offset = 0;
    switch(segment[0]){
        case 'L':
            col_offset = -1;
            break;
        case 'R':
            col_offset = 1;
            break;
        case 'U':
            row_offset = 1;
            break;
        case 'D':
            row_offset = -1;
            break;
    }
    // get length from segment
    int length = atoi(++segment);
    // draw segment on board
    for(int i=0; i<length; i++){
        if(pipe == 1 && !panel[row][col]){
            // printf("drawing segment #%d\n", totalDistance);
            panel[row][col] = totalDistance;
        }
        else if(pipe == 2 && panel[row][col]){
            // overlap found, calculate distance
            // printf("checking segment #%d\n", totalDistance);
            int distance = panel[row][col] + totalDistance;
            if(distance < minDistance)
                minDistance = distance;
        }
        totalDistance++;
        row += row_offset;
        col += col_offset;
    }
}

int main(int argc, char *argv[]){
    // extract segments and draw
    for(char i=1; i<argc; i++){
        row = PANEL_SIZE/2;
        col = PANEL_SIZE/2;
        panel[row][col] = 0;
        totalDistance = 0;
        char *token = strtok(argv[i], ",");
        for(int j=0; token != NULL; j++){
            drawSegment(token, j, i);
            token = strtok(NULL, ",");
        }
    }
    printf("Minimum traveling distance: %d\n", minDistance);
    return 0;
}
