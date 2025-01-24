#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "loadmem.h"


/*
* loadmem.c / Practice Assignment / Memory Layout and Memory Management
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan , 2025 
*
*/

// Maximum length of each line in the file
#define MAX_LINE_LENGTH 1024  

// Function to allocate memory for a dyn_block of given size
dyn_block* alloc_dyn_block(int size) {
    dyn_block *block = (dyn_block*)malloc(sizeof(dyn_block));
    if (block == NULL) {
        printf("Memory allocation for dyn_block failed.\n");
        return NULL;
    }

    block->data = (int*)malloc(size * sizeof(int));
    if (block->data == NULL) {
        printf("Memory allocation for data array failed.\n");
        free(block);
        return NULL;
    }

    block->size = size;
    return block;
}

// Function to store integers into the dynamic block
void store_mem_blk(dyn_block *block, int *data, int size) {
    if (block == NULL || block->data == NULL) {
        printf("Invalid block provided.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        block->data[i] = data[i];
    }

    printf("Data successfully stored in the dynamic block.\n");
}

// Function to read integers from a file and store them in dynamic blocks
void read_file_and_store_blocks(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int numbers[MAX_LINE_LENGTH];
        int count = 0;

        // Read integers from the line
        char *token = strtok(line, " \n");
        while (token != NULL && count < MAX_LINE_LENGTH) {
            numbers[count++] = atoi(token);
            token = strtok(NULL, " \n");
        }

        // Allocate dynamic block for the read integers
        dyn_block *block = alloc_dyn_block(count);
        if (block == NULL) {
            printf("Failed to allocate memory for block.\n");
            fclose(file);
            return;
        }

        // Store the read numbers in the block
        store_mem_blk(block, numbers, count);

        // Print stored numbers to verify
        printf("Stored numbers: ");
        for (int i = 0; i < block->size; i++) {
            printf("%d ", block->data[i]);
        }
        printf("\n");

        // Free the allocated block memory
        free(block->data);
        free(block);
    }

    fclose(file);
}

int main(void) {
    // Provide the filename (ensure the file exists in the same directory)
    const char *filename = "blocks.data";

    if (access(filename, F_OK) != 0) {
    printf("Error: File %s not found.\n", filename);
    return 1;
	}	
    
    printf("Reading and storing integers from file: %s\n", filename);
    read_file_and_store_blocks(filename);

    printf("Memory processing completed.\n");
    return 0;
}

