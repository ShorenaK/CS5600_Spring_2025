#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

/*
* cipher.c / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025
*
*/

#define MAX_TEXT_LENGTH 256

int main(int argc, char *argv[]) {
    char input[MAX_TEXT_LENGTH];

    while (fgets(input, sizeof(input), stdin)) {
        char result[MAX_TEXT_LENGTH];
        pbEncode(input, result);
        printf("%s\n", result);
    }
    
    return 0;
}

