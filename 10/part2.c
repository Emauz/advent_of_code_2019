// Eric Moss
// Advent of code 2019
// Day 10 part 2

// Find the 200th asteroid we can blast

// input: <file containing input>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_WIDTH 64
#define MAX_HEIGHT 64
#define BIG_ANGLE 1080*1080

// height and width available to all
// repeatedly passing as parameters is a waste of stackframe space
int height, width;

typedef struct Location{
    bool isAsteroid;
    bool beenMarked;
    int row;
    int col;
    int visibleAsteroids;
    int depth;
    int deleteOrder;
} Location;

Location *createLocation(bool isAsteroid, int row, int col){
    Location *newLocation = malloc(sizeof(Location));
    newLocation->isAsteroid = isAsteroid;
    newLocation->beenMarked = false;
    newLocation->row = row;
    newLocation->col = col;
    newLocation->visibleAsteroids = -1;
    newLocation->depth = isAsteroid -1;
    newLocation->deleteOrder = -1;
    return newLocation;
}

void printField(Location *field[height][width]){
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(field[i][j]->depth > -1){
                printf("%d", field[i][j]->depth);
            }
            else{
                putc(field[i][j]->isAsteroid?'#':'.', stdout);
            }
        }
        putc('\n', stdout);
    }
    //printf("Height: %d, width: %d\n", height, width);
    puts("");
}

double angleBetween(Location *start, Location *end){
    if(end == NULL)
        return BIG_ANGLE;
    double verticalDisplacement = (double)(end->row - start->row);
    double horizontalDisplacement = (double)(end->col - start->col);
    //double verticalAngle = atan2(1, 0);
    double angle = atan2(verticalDisplacement, horizontalDisplacement);
    angle *= (180/M_PI); // convert to degrees
    if(angle < 0)
        angle += (360);
    angle += 90;
    if(angle >=360)
        angle -= 360;
    return angle;
}

bool validCoords(int row, int col){
    return(row>=0 && row<height && col>=0 && col<width);
}

// returns true if ray contains asteroid
bool enumerateRay(Location *field[height][width], Location *src, Location *dst){
    int depth = 1;
    int deltaRow = dst->row - src->row;
    int deltaCol = dst->col - src->col;
    int row = src->row + deltaRow;
    int col = src->col + deltaCol;
    bool foundValidAsteroid = false;
    while(validCoords(row, col)){
        field[row][col]->beenMarked = true;
        if(field[row][col]->isAsteroid && field[row][col]->depth==-1){
            field[row][col]->depth = depth++;
            foundValidAsteroid = true;
        }
        row += deltaRow;
        col += deltaCol;
    }
    return foundValidAsteroid;
}

// returns number of rays with asteroids it finds
int markInACircle(Location *field[height][width], Location *src){
    // Iterate through radii of circles around src
    int maxRadius = height+width;
    int validRays = 0;
    //char charRep = 'A';
    for(int radius=1; radius<maxRadius; radius++){
        // begin pointed up
        int row = src->row - radius;
        int col = src->col;
        // loop on each side
        for(int k=0; k<4; k++){
            int deltaRow = (k<2) ? 1 : -1;
            int deltaCol = (k%3==0) ? 1 : -1;
            do{
                if(validCoords(row, col) && !field[row][col]->beenMarked){
                    // enumerate ray out from source location
                    if(enumerateRay(field, src, field[row][col]))
                        validRays++;
                }
                row += deltaRow;
                col += deltaCol;
            }while(abs(row-src->row) != radius && abs(col-src->col) != radius);
        }
    }
    return validRays;
}

void resetField(Location *field[height][width]){
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            field[i][j]->depth = -1;
            field[i][j]->beenMarked = false;
        }
    }
}

// marks all asteroids with what number they will be to be lazered
void markDeleteOrder(Location *field[height][width], Location *src){
    src->deleteOrder = 0;
    int idx = 1;
    Location *nextToDelete;
    double smallestAngle;
    double currentAngle;
    do{
        nextToDelete = NULL;
        smallestAngle = BIG_ANGLE;
        // scan all available asteroids for next to delete
        for(int row=0; row<height; row++){
            for(int col=0; col<width; col++){
                Location *currentLocation = field[row][col];
                if(currentLocation->isAsteroid && 
                    currentLocation->deleteOrder==-1){
                    //printf("%d,%d ", currentLocation->row, currentLocation->col);
                    currentAngle = angleBetween(src, currentLocation);
                    currentAngle += (360*currentLocation->depth);
                    //printf("%lf\n", currentAngle);
                    if(currentAngle < smallestAngle){
                        smallestAngle = currentAngle;
                        nextToDelete = currentLocation;
                    }
                }
            }
        }
        // found next location to delete
        if(nextToDelete!=NULL){
            //printf("row: %d, col: %d, order: %d\n", nextToDelete->row, 
                //nextToDelete->col, idx);
            nextToDelete->deleteOrder = idx++;
        }
    }while(nextToDelete != NULL);
}

Location *nthAsteroid(Location *field[height][width], Location *src, int n){
    int deleteIdx = 0;
    markInACircle(field, src);
    markDeleteOrder(field, src);
    /*
    for(int row=0; row<height; row++){
        for(int col=0; col<width; col++){
            if(field[row][col]->isAsteroid){
                printf("%2d ", field[row][col]->deleteOrder);
            } else
                printf(" . ");
        }
        puts("");
    }
    */
    // find the given asteroid
    for(int row=0; row<height; row++){
        for(int col=0; col<width; col++){
            if(field[row][col]->deleteOrder == n){
                return field[row][col];
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    // determine field width from first line
    char board[MAX_HEIGHT][MAX_WIDTH];
    FILE *openedFile = fopen(argv[1], "r");
    fscanf(openedFile, "%s", board[0]);
    width = strlen(board[0]);
    // create rest of board
    height = 1;
    while(fscanf(openedFile, "%s", board[height]) == 1){
        height++;
    }
    // now we can generate the actual asteroid field
    Location *field[height][width];
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            field[i][j] = createLocation(board[i][j]=='#', i, j);
        }
    }
    // and calculate how many asteroids are visible for each asteroid
    int maxVisibleAsteroids = 0;
    int targetRow = -1;
    int targetCol = -1;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            if(field[i][j]->isAsteroid){
                field[i][j]->visibleAsteroids=markInACircle(field, field[i][j]);
                if(field[i][j]->visibleAsteroids > maxVisibleAsteroids){
                    maxVisibleAsteroids = field[i][j]->visibleAsteroids;
                    targetRow = i;
                    targetCol = j;
                }
            }
            resetField(field);
        }
    }
    printf("Station is at row:%d, col:%d, with %d visible asteroids\n", 
        targetRow, targetCol, maxVisibleAsteroids);
    // find 200th asteroid to be hit by lazer
    Location *a200 = nthAsteroid(field, field[targetRow][targetCol], 200);
    printf("200th asteroid to be hit is row:%d, col:%d\n", a200->row, 
        a200->col);
    /*
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            printf("%3.lf ", angleBetween(field[3][5], field[i][j])); 
        }
        printf("\n");
    }
    */
    printf("Answer to problem is :%d\n", (a200->col*100)+(a200->row));
}
