// Eric Moss
// Advent of code 2019
// day 6 part 2

// calculate total of orbital transfers required
// input: paste input into stdin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PLANET_ID_SIZE 3
#define ORBIT_DEF_SIZE 8
#define ALPHABET_BIT_SIZE 6
#define NUM_CHILDREN 64

#define alphaBase(X) (1 << (ALPHABET_BIT_SIZE * X))

typedef struct Planet{
    char id[PLANET_ID_SIZE+1];
    struct Planet *parent;
    struct Planet **children;
    bool checked;
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
        planets[idx]->checked = false;
    }
    return planets[idx];
}

void addOrbit(Planet *parentPlanet, Planet *childPlanet){
    int i=0;
    while(parentPlanet->children[i] != NULL)
        i++;
    parentPlanet->children[i] = childPlanet;
    childPlanet->parent = parentPlanet;
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

int findSanta(Planet *src, Planet *dst, Planet **planets){
    src->checked = true;
    if(src == dst)
        return 0;
    if(src->children == NULL)
        return -1;
    Planet *currentChild;
    int santaDistance = 0;
    for(int i=0; NULL != (currentChild = src->children[i]); i++){
        if(!currentChild->checked){
            santaDistance = findSanta(currentChild, dst, planets);
            if(santaDistance != -1)
                return (santaDistance + 1);
        }
    }
    if(!src->parent->checked){
        santaDistance = findSanta(src->parent, dst, planets);
        return santaDistance + 1;
    }
    return -1;
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
    Planet *you = getPlanet("YOU", planets);
    Planet *san = getPlanet("SAN", planets);
    printf("Santa distance: %d\n", findSanta(you, san, planets) - 2);
}
