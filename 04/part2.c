// cracking a secure container's password
// this time, only two numbers are allowed to repeat
// input: <beginning of range> <end of range>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_LENGTH 6

bool increasingOrder(int password[]){
    for(int i=0; i<NUMBER_LENGTH-1; i++){
        if(password[i] > password[i+1])
            return false;
    }
    return true;
}

bool containsDouble(int password[]){
    // check first double
    if(password[0] == password[1] && password[0] != password[2]){
        return true;
    }
    // check last double
    if(password[NUMBER_LENGTH-2] == password[NUMBER_LENGTH-1] 
        && password[NUMBER_LENGTH-3] != password[NUMBER_LENGTH-2]){
        return true;
    }
    for(int i=1; i<NUMBER_LENGTH-2; i++){
        if(password[i] == password[i+1] &&
            password[i-1] != password[i] && 
            password[i+2] != password[i]){
            return true;
        }
    }
    return false;
}

bool validatePassword(int password){
    int passwordArray[NUMBER_LENGTH];
    for(int i=NUMBER_LENGTH-1; i>=0; i--){
        passwordArray[i] = password % 10;
        password = (password - passwordArray[i])/10;
    }
    return increasingOrder(passwordArray) && containsDouble(passwordArray);
}

int main(int argc, char *argv[]){
    int begin = atoi(argv[1]);
    int end = atoi(argv[2]);
    int totalFound = 0;
    for(int i=begin; i<end; i++){
        if(validatePassword(i))
            totalFound++;
    }
    /*
    if(validatePassword(122456))
        totalFound++;
    */
    printf("Total found: %d\n", totalFound);
}
