// cracking a secure container's password
// input: <beginning of range> <end of range>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool validatePassword(int password){
    int curNum;
    int prevNum = 100;
    bool repeating = false;
    for(int i=0; i<6; i++){
        curNum = password % 10;
        if(curNum == prevNum)
            repeating = true;
        if(curNum > prevNum)
            return false;
        prevNum = curNum;
        password = (password - curNum) / 10;
    }
    return repeating;
}

int main(int argc, char *argv[]){
    int begin = atoi(argv[1]);
    int end = atoi(argv[2]);
    int totalFound = 0;
    for(int i=begin; i<end; i++){
        if(validatePassword(i))
            totalFound++;
    }
    printf("Total found: %d\n", totalFound);
}
