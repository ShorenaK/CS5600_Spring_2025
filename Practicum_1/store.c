#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

/*
* store.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/

/* 
* The function in this file simulates secondary storage(disk)operation.
*/

// Stores a message in a file
void store_msg(Message* msg) {
    FILE* file = fopen(MSG_FILE, "ab");
    if (!file) {
        printf("Error opening file for writing\n");
        return;
    }
    fwrite(msg, sizeof(Message), 1, file);
    fclose(file);
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
