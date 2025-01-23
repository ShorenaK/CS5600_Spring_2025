#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

/*
* cipher.c / Practice Assignment / Random Number Generation and Encryption
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan , 2025 
*
*/

// This program handles command-line input and performs encryption 
// or decryption using the Polybius Square cipher.

#define MAX_TEXT_LENGTH 256

void printUsage(const char *programName) {
    printf("Usage: %s -e \"text to encrypt\"\n", programName);
    printf("       %s -d \"encrypted text\"\n", programName);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    char result[MAX_TEXT_LENGTH];

    if (strcmp(argv[1], "-e") == 0) {
        pbEncode(argv[2], result);
        printf("Encoded: %s\n", result);
    } else if (strcmp(argv[1], "-d") == 0) {
        pbDecode(argv[2], result);
        printf("Decoded: %s\n", result);
    } else {
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}

