// Find closest intersection between two wires
// input: <first wire spec> <second wire spec>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PANEL_SIZE 30000

char panel[PANEL_SIZE][PANEL_SIZE];
int row;
int col;;
int minDistance = 2048;

// Draw a single segment of wire on the panel
void drawSegment(char *segment, char representation){
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
        if(!panel[row][col]){
            panel[row][col] = representation;
        }
        else if(panel[row][col] != representation){
            // overlap found, calculate distance
            panel[row][col] = representation;
            int distance = abs(row-(PANEL_SIZE/2)) + abs(col-(PANEL_SIZE/2));
            if(distance < minDistance)
                minDistance = distance;
        }
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
        char *token = strtok(argv[i], ",");
        while(token != NULL){
            drawSegment(token, i);
            token = strtok(NULL, ",");
        }
    }
    printf("Minimum Manhattan distance: %d\n", minDistance);
    return 0;
}
