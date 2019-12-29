// Eric Moss
// Advent of code 2019
// Day 14 part 1

// Calculate ORE needed to generate 1 FUEL

// input: <FILE containing recipe>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHEM_NAME_SIZE 6
#define MAX_INPUTS 16
#define MAX_REACTIONS 128

typedef struct{
    int quantity;
    char chemical[MAX_CHEM_NAME_SIZE];
}Substance;

typedef struct{
    Substance *inputs[MAX_INPUTS];
    Substance *output;
    int available;
}Reaction;

Substance *parseSubstance(char *substance){
    Substance *output = malloc(sizeof(Substance));
    int quantity;
    char chemical[MAX_CHEM_NAME_SIZE];
    sscanf(substance, "%d %s", &quantity, chemical);
    output->quantity = quantity;
    strcpy(output->chemical, chemical);
    return output;
}

void printSubstance(Substance *input){
    printf("%d %s", input->quantity, input->chemical);
}

void printReaction(Reaction *reaction){
    int inputNum = 0;
    while(reaction->inputs[inputNum]){
        printSubstance(reaction->inputs[inputNum++]);
        if(reaction->inputs[inputNum]){
            printf(",");
        }
        printf(" ");
    }
    printf("=> "); 
    printSubstance(reaction->output);
    puts("");
}

Reaction *getReaction(Reaction *reactions[MAX_REACTIONS], char *chemical){
    for(int i=0; reactions[i] != NULL; i++){
        if(!strcmp(reactions[i]->output->chemical, chemical)){
            return reactions[i];
        }
    }
    return NULL;
}

Reaction *parseReaction(char *reaction){
    Reaction *newReaction = malloc(sizeof(Reaction));
    // split into input and output
    char *in = strtok(reaction, "=>");
    char *out = strtok(NULL, "=>");
    // parse input substances
    char *buf = strtok(in, ",");
    int numInputs = 0;
    while(buf != NULL){
        newReaction->inputs[numInputs] = parseSubstance(buf);
        numInputs++;
        buf = strtok(NULL, ",");
    }
    // parse output substance
    newReaction->output = parseSubstance(out);
    return newReaction;
}

void runReaction(Reaction *reactions[MAX_REACTIONS], Reaction *reaction);

// gets cost (in ore) of producing a given substance
int getCost(Reaction *reactions[MAX_REACTIONS], Substance *substance){
    // if this is ore, return cost
    if(!strcmp(substance->chemical, "ORE")){
        return substance->quantity;
    }
    int cost = 0;
    Reaction *targetReaction = getReaction(reactions, substance->chemical);
    // while we still need to generate more of our target chemical
    while(targetReaction->available < substance->quantity){
        int inputNum = 0;
        while(targetReaction->inputs[inputNum] != NULL){
            cost += getCost(reactions, targetReaction->inputs[inputNum]);
            inputNum++;
        }
        targetReaction->available += targetReaction->output->quantity;
    }
    targetReaction->available -= substance->quantity;
    return cost;
}

int main(int argc, char *argv[]){
    FILE *openedFile = fopen(argv[1], "r");
    char buf[BUFSIZ];
    // construct all reactions
    int numReactions = 0;
    Reaction *reactions[MAX_REACTIONS] = {NULL};
    while(NULL != fgets(buf, BUFSIZ, openedFile)){
        //puts(buf);
        reactions[numReactions] = parseReaction(buf);
        numReactions++;
    }
    int fuelCost = getCost(reactions, parseSubstance("1 FUEL"));
    printf("cost for 1 FUEL: %d\n", fuelCost);
}
