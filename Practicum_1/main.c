#include <stdio.h>
#include "message.h"
#include "storage.h"


/*
* main.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/

// This file tests the entire system.

int main() {
    // Create a new message
    Message* msg1 = create_msg(1, "Shorena", "Mike", "Good morning, Mike!", 0);
    if (msg1 == NULL) {
        printf("Error: Failed to create message.\n");
        return -1;
    }

    // Store the message
    if (store_msg(msg1) != 0) {
        printf("Error: Failed to store message.\n");
        free_msg(msg1);
        return -1;
    }

    printf("Message stored successfully.\n");

    // Free the dynamically allocated message (already stored on disk)
    free_msg(msg1);

    // Retrieve the message by ID
    Message* retrieved_msg = retrieve_msg(1);
    if (retrieved_msg == NULL) {
        printf("Error: Message not found.\n");
        return -1;
    }

    // Display the retrieved message
    printf("\nMessage Retrieved:\n");
    print_msg(retrieved_msg);

    // Free allocated memory for retrieved message
    free_msg(retrieved_msg);

    return 0;
}
