/*
* proccess_maneger.c / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025
*
*/

/* process_manager.c - Handles multi-processing encryption */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

#define MAX_WORD_LENGTH 100

int main() {
    Queue *q = createQueue();

    FILE *file = fopen("strings.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", buffer) == 1) {
        enqueue(q, buffer);
    }

    fclose(file);

    printf("Words stored in queue:\n");
    printQueue(q);

    freeQueue(q);
    return 0;
}
