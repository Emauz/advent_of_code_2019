// rocket counter fuel upper thing

#include <stdio.h>
#include <stdlib.h>

int main(void){
    int runningTotal = 0;
    int mass;
    while(scanf("%d", &mass) != EOF){
        int fuel = (mass / 3) - 2;
        runningTotal += fuel;
    }
    printf("Total fuel required: %d\n", runningTotal);
}
