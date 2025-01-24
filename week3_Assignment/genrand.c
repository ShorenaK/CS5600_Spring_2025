#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
* genrand.c / Practice Assignment / Random Number Generation and Encryption
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan , 2025 
*
*/

// Linear Congruential Generator parameters
#define A 1664525
#define C 1013904223
#define M 4294967296  // 2^32

// Function to generate a pseudo-random number within a given range
int genRand(int min, int max) {
    static unsigned int seed = 0;
    if (seed == 0) {
     	   // Initialize seed
	    seed = (unsigned int)time(NULL);  
    }
    seed = (A * seed + C) % M;
    return (seed % (max - min + 1)) + min;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <number_of_randoms> <file_name> [-a]\n", argv[0]);
        return 1;
    }

    int numRandoms = atoi(argv[1]);
    if (numRandoms <= 0) {
        fprintf(stderr, "Error: The number of random values must be greater than 0.\n");
        return 1;
    }

    char *fileName = argv[2];
    int appendMode = (argc == 4 && strcmp(argv[3], "-a") == 0) ? 1 : 0;

    FILE *file = fopen(fileName, appendMode ? "a" : "w");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < numRandoms; i++) {
	// Generate random number in range [1,100]
        int randomNum = genRand(1, 100);  
        fprintf(file, "%d\n", randomNum);
    }

    fclose(file);
    printf("Successfully generated %d random numbers in %s mode.\n", numRandoms, appendMode ? "append" : "overwrite");
    return 0;
}
