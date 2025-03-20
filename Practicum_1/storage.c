#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

/*
* storage.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/

/* 
* The function in this file simulates secondary storage(disk)operation.
*/

// Stores a message in a file
int store_msg(Message* msg) {
    FILE* file = fopen(MSG_FILE, "ab");
    if (!file) {
        printf("Error opening file for writing\n");
        // Error during file write
        return -1;
    }
    size_t written = fwrite(msg, sizeof(Message), 1, file);
    fclose(file);
    
    
    if (written != 1) {
        printf("Error: Failed to write message to file.\n");
        return -1; 
    }
    // Success
    return 0; 
}

// Retrieves a message by ID from the file
Message* retrieve_msg(int id) {
    FILE* file = fopen(MSG_FILE, "rb");
    if (!file) {
        printf("Error opening file for reading\n");
        return NULL;
    }

    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    while (fread(msg, sizeof(Message), 1, file)) {
        if (msg->id == id) {
            fclose(file);
            return msg;
        }
    }

    fclose(file);
    free(msg);
   // if message not found returns null
    return NULL;
}
