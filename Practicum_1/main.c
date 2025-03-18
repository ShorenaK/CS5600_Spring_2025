#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
* main.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/


#include <stdio.h>
#include "message.h"

int main() {
    // Create a new message
    Message* msg1 = create_msg(1, "Alice", "Bob", "Hello, Bob!");
    if (msg1 == NULL) {
        printf("Failed to create message.\n");
        return -1;
    }

    // Store the message
    if (store_msg(msg1) != 0) {
        printf("Failed to store message.\n");
        return -1;
    }

    printf("Message stored successfully.\n");

    // Retrieve the message by ID
    Message* retrieved_msg = retrieve_msg(1);
    if (retrieved_msg == NULL) {
        printf("Message not found.\n");
        return -1;
    }

    // Display the retrieved message
    printf("Message Retrieved:\n");
    printf("ID: %d\n", retrieved_msg->id);
    printf("Sender: %s\n", retrieved_msg->sender);
    printf("Receiver: %s\n", retrieved_msg->receiver);
    printf("Content: %s\n", retrieved_msg->content);
    printf("Delivered: %d\n", retrieved_msg->delivered);

    // Free dynamically allocated memory
    free(msg1);
    free(retrieved_msg);

    return 0;
}

