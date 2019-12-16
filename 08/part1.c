// Eric Moss
// Advent of code 2019
// Day 8 part 1

// input: <file containing input>

// Space image format interpreter

#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH 25
#define IMAGE_HEIGHT 6
#define MAX_IMAGE_DEPTH 101

int main(int argc, char *argv[]){
    FILE *inputFile = fopen(argv[1], "r");
    char imageData[MAX_IMAGE_DEPTH][IMAGE_HEIGHT][IMAGE_WIDTH];
    int actualDepth = -1;
    char currentChar;
    // construct image data from file
    for(int i=0; !feof(inputFile); i++){
        actualDepth++;
        for(int j=0; j < IMAGE_HEIGHT; j++){
            for(int k=0; k < IMAGE_WIDTH; k++){
                imageData[i][j][k] = (getc(inputFile)-48); // convert ascii
            }
        }
    }
    fclose(inputFile);

    int minZeroes = IMAGE_HEIGHT * IMAGE_WIDTH;
    int targetOnes = 0;
    int targetTwos = 0;
    int targetLayer = 0;
    for(int i=0; i < actualDepth; i++){
        int currentZeroes = 0;
        int currentOnes = 0;
        int currentTwos = 0;
        for(int j=0; j < IMAGE_HEIGHT; j++){
            for(int k=0; k < IMAGE_WIDTH; k++){
                switch(imageData[i][j][k]){
                    case 0:
                        currentZeroes++;
                        break;
                    case 1:
                        currentOnes++;
                        break;
                    case 2:
                        currentTwos++;
                        break;
                }
            }
        }
        if(currentZeroes < minZeroes){
            minZeroes = currentZeroes;
            targetOnes = currentOnes;
            targetTwos = currentTwos;
            targetLayer = i;
        }
    }
    printf("Min zeroes: layer %d with %d zeroes,\n", targetLayer, minZeroes);
    printf("%d ones and %d twos.\n", targetOnes, targetTwos);
    printf("This results in an answer of %d!\n", targetOnes * targetTwos);
}
