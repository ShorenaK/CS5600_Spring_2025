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

//  This program handles command-line input and performs encryption
// or decryption using the Polybius Square cipher.

#define MAX_TEXT_LENGTH 256

void printUsage(void) {
    printf("Usage: cipher -e to encrypt or cipher -d to decrypt\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        void printUsage(void);
        return 1;
    }

    char input[MAX_TEXT_LENGTH];
    char result[MAX_TEXT_LENGTH];

    // Check for encryption or decryption mode
    if (strcmp(argv[1], "-e") == 0) {
        // Encoding mode
        while (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0; 
            pbEncode(input, result);
            printf("%s\n", result);
 	}
    } else if (strcmp(argv[1], "-d") == 0) {
        // Decoding mode
        while (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0; 
            pbDecode(input, result);
            printf("%s\n", result);
        }
    } else {
        void printUsage(void);
        return 1;
    }
    return 0;
}
