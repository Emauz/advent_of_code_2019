// Eric Moss
// Advent of code 2019
// Day 12 part 2

// Calculate 3d velocities of Jupiter's 4 largest moons

// input: <file containing input>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PLANETS 5
#define DATA_FORMAT "<x=%3d, y=%3d, z=%3d>"

typedef struct{
    int x;
    int y;
    int z;
    int dx;
    int dy;
    int dz;
}Planet;

Planet *createPlanet(int x, int y, int z){
    Planet *output = malloc(sizeof(Planet));
    output->x = x;
    output->y = y;
    output->z = z;
    output->dx = 0;
    output->dy = 0;
    output->dz = 0;
    return output;
}

void printPlanets(int numPlanets, Planet *planets[numPlanets]){
    for(int i=0; i<numPlanets; i++){
        printf("pos=");
        printf(DATA_FORMAT, planets[i]->x, planets[i]->y, planets[i]->z);
        printf(", vel=");
        printf(DATA_FORMAT, planets[i]->dx, planets[i]->dy, planets[i]->dz);
        puts("");
    }
}

int calculateEnergy(int numPlanets, Planet *planets[numPlanets]){
    int totalEnergy = 0;
    for(int i=0; i<numPlanets; i++){
        int pot = abs(planets[i]->x)+abs(planets[i]->y)+abs(planets[i]->z);
        int kin = abs(planets[i]->dx)+abs(planets[i]->dy)+abs(planets[i]->dz);
        totalEnergy += (pot * kin);
    }
    return totalEnergy;
}

void applyGravity(Planet *a, Planet *b, char dim){
    // pulls are always for planet a.
    int pull;
    switch(dim){
        case 'x':
            if(a->x == b->x)
                pull = 0;
            else
                pull = (a->x < b->x) ? 1 : -1;
            a->dx += pull;
            b->dx -= pull;
            break;
        case 'y':
            if(a->y == b->y)
                pull = 0;
            else
                pull = (a->y < b->y) ? 1 : -1;
            a->dy += pull;
            b->dy -= pull;
            break;
        case 'z':
            if(a->z == b->z){
                pull = 0;
            }
            else{
                pull = (a->z < b->z) ? 1 : -1;
            }
            a->dz += pull;
            b->dz -= pull;
            break;
    }
}

void move(Planet *planet, char dim){
    switch(dim){
        case 'x':
            planet->x += planet->dx;
            break;
        case 'y':
            planet->y += planet->dy;
            break;
        case 'z':
            planet->z += planet->dz;
            break;
    }
}

void simulateStep(int numPlanets, Planet *planets[numPlanets], char dim){
    for(int i=0; i<numPlanets-1; i++){
        for(int j=i; j<numPlanets; j++){
            applyGravity(planets[i], planets[j], dim);
        }
    }
    for(int i=0; i<numPlanets; i++){
        move(planets[i], dim);
    }
}

bool equalDim(int numPlanets, Planet *a[numPlanets], Planet *b[numPlanets], 
    char dim){
    bool equal = true;
    for(int i=0; i<numPlanets; i++){
        switch(dim){
            case 'x':
                if(a[i]->x != b[i]->x || a[i]->dx != b[i]->dx)
                    equal = false;
                break;
            case 'y':
                if(a[i]->y != b[i]->y || a[i]->dy != b[i]->dy)
                    equal = false;
                break;
            case 'z':
                if(a[i]->z != b[i]->z || a[i]->dz != b[i]->dz)
                    equal = false;
                break;
        }
    }
    return equal;
}

Planet **copyPlanets(int numPlanets, Planet *planets[numPlanets]){
    Planet **output = malloc(sizeof(Planet*) * MAX_PLANETS);
    for(int i=0; i<numPlanets; i++){
        output[i] = malloc(sizeof(Planet));
        output[i]->x = planets[i]->x;
        output[i]->y = planets[i]->y;
        output[i]->z = planets[i]->z;
        output[i]->dx = planets[i]->dx;
        output[i]->dy = planets[i]->dy;
        output[i]->dz = planets[i]->dz;
    }
    return output;
}

int getPeriod(int numPlanets, Planet *planets[numPlanets], char dim){
    Planet **tmpPlanets = copyPlanets(numPlanets, planets);

    int period = 0;
    do{
        simulateStep(numPlanets, tmpPlanets, dim);
        period++;
    }while(!equalDim(numPlanets, planets, tmpPlanets, dim));
    return period;
}

long lcm(long a, long b){
    long gcd;
    for(long i=1; i<=a && i<=b; i++){
        if(a%i==0 && b%i==0)
            gcd = i;
    }
    return (a*b)/gcd;
}


int main(int argc, char *argv[]){
    // read input file
    FILE *openedFile = fopen(argv[1], "r");
    int x, y, z;
    char buf[BUFSIZ];
    int numPlanets = 0;
    Planet *planets[MAX_PLANETS];
    while(NULL != fgets(buf, BUFSIZ, openedFile)){
        sscanf(buf, DATA_FORMAT, &x, &y, &z);
        planets[numPlanets++] = createPlanet(x, y, z);
    }
    fclose(openedFile);

    int xperiod = getPeriod(numPlanets, planets, 'x');
    int yperiod = getPeriod(numPlanets, planets, 'y');
    int zperiod = getPeriod(numPlanets, planets, 'z');
    printf("xperiod: %d\n", xperiod);
    printf("yperiod: %d\n", yperiod);
    printf("zperiod: %d\n", zperiod);
    printf("system period: %ld\n", lcm(lcm(xperiod, yperiod), zperiod));
}
