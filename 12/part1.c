// Eric Moss
// Advent of code 2019
// Day 12 part 1

// Calculate 3d velocities of Jupiter's 4 largest moons

// input: <file containing input> <number of steps to simulate>

#include <stdio.h>
#include <stdlib.h>

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

void applyGravity(Planet *a, Planet *b){
    // pulls are always for planet a.
    int pullX, pullY, pullZ;
    // x direction
    if(a->x == b->x)
        pullX = 0;
    else
        pullX = (a->x < b->x) ? 1 : -1;
    // y direction
    if(a->y == b->y)
        pullY = 0;
    else
        pullY = (a->y < b->y) ? 1 : -1;
    // z direction
    if(a->z == b->z)
        pullZ = 0;
    else
        pullZ = (a->z < b->z) ? 1 : -1;
    // apply pulls to planets
    a->dx += pullX;
    a->dy += pullY;
    a->dz += pullZ;
    b->dx -= pullX;
    b->dy -= pullY;
    b->dz -= pullZ;
}

void move(Planet *planet){
    planet->x += planet->dx;
    planet->y += planet->dy;
    planet->z += planet->dz;
}

void simulateStep(int numPlanets, Planet *planets[numPlanets]){
    for(int i=0; i<numPlanets-1; i++){
        for(int j=i; j<numPlanets; j++){
            applyGravity(planets[i], planets[j]);
        }
    }
    for(int i=0; i<numPlanets; i++){
        move(planets[i]);
    }
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

    // simulate planet behavior
    int numSteps = atoi(argv[2]);
    for(int i=0; i<numSteps; i++){
        simulateStep(numPlanets, planets);
    }
    printf("Total energy: %d\n", calculateEnergy(numPlanets, planets));
}
