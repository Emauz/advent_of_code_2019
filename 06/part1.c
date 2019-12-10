// Eric Moss
// Advent of code 2019
// day 6 part 1

// calculate total of direct + indirect orbits
// input: paste input into stdin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLANET_ID_SIZE 3
#define ORBIT_DEF_SIZE 8
#define ALPHABET_BIT_SIZE 6
#define NUM_CHILDREN 64

#define alphaBase(X) (1 << (ALPHABET_BIT_SIZE * X))

typedef struct Planet{
    char id[PLANET_ID_SIZE+1];
    struct Planet **children;
} Planet;

int getIdx(char *id){
    return ((id[2]-48) * alphaBase(0)) +
        ((id[1]-48) * alphaBase(1)) +
        ((id[0]-48) * alphaBase(2));
}

Planet *getPlanet(char *id, Planet **planets){
    int idx = getIdx(id);
    if(planets[idx] == NULL){
        planets[idx] = calloc(sizeof(Planet), 1);
        memcpy(planets[idx]->id, id, PLANET_ID_SIZE+1);
        planets[idx]->children = calloc(sizeof(Planet*), NUM_CHILDREN);
    }
    return planets[idx];
}

void addOrbit(Planet *parentPlanet, Planet *childPlanet){
    int i=0;
    while(parentPlanet->children[i] != NULL)
        i++;
    parentPlanet->children[i] = childPlanet;
}

int countSubOrbits(Planet *planet, int parents){
    if(planet->children == NULL)
        return parents;
    int subOrbits = parents;
    for(int i=0; planet->children[i] != NULL; i++){
        subOrbits += countSubOrbits(planet->children[i], parents + 1);
    }
    return subOrbits;
}

int main(void){
    Planet **planets = calloc(sizeof(Planet*), alphaBase(3));
    char orbit[ORBIT_DEF_SIZE];
    while(EOF != (scanf("%s", &orbit))){
        char *parent = strtok(orbit, ")");
        char *child = strtok(NULL, "");
        Planet *parentPlanet = getPlanet(parent, planets);
        Planet *childPlanet = getPlanet(child, planets);
        addOrbit(parentPlanet, childPlanet);
    }
    printf("SubOrbits found: %d\n", 
        countSubOrbits(getPlanet("COM", planets), 0));
}
