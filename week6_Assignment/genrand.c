#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
* genrand.c / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025, Feb 2025
*
*/

// Linear Congruential Generator parameters (for reproducibility)
#define A 1664525
#define C 1013904223
#define M 4294967296  // 2^32


// Function to generate a pseudo-random number within a given range
int genRand(int min, int max) {
    static unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL);
    }
    seed = (A * seed + C) % M;
    return (seed % (max - min + 1)) + min;
}

// Function to generate a random string of length n
void genRandomString(char *str, int length) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
        str[i] = alphabet[genRand(0, (sizeof(alphabet) - 2))];
    }
    str[length] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <number_of_strings> <file_name> [-a]\n", argv[0]);
        return 1;
    }

    int numStrings = atoi(argv[1]);
    if (numStrings <= 0) {
        fprintf(stderr, "Error: The number of strings must be greater than 0.\n");
        return 1;
    }

    char *fileName = argv[2];
    int appendMode = (argc == 4 && strcmp(argv[3], "-a") == 0) ? 1 : 0;

    FILE *file = fopen(fileName, appendMode ? "a" : "w");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    // Genrating worids 10 characters -- fixed 
    char randomString[10];  

    for (int i = 0; i < numStrings; i++) {
        genRandomString(randomString, 10);
        fprintf(file, "%s\n", randomString);
    }

    fclose(file);
    printf("Successfully generated %d random strings in %s mode.\n", numStrings, appendMode ? "append" : "overwrite");
    return 0;
}

