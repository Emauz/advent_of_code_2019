// rocket counter fuel upper thing
// but this time it's part 2
// input: paste newline separated input into running program

#include <stdio.h>
#include <stdlib.h>

int main(void){
    int runningTotal = 0;
    int mass;
    while(scanf("%d", &mass) != EOF){
        int fuel = (mass / 3) - 2;
        runningTotal += fuel;
        mass = fuel;
        while((fuel=(mass/3)-2)>0){
            runningTotal += fuel;
            mass = fuel;
        }
    }
    printf("Total fuel required: %d\n", runningTotal);
}
