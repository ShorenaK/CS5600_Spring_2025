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

// This file tests the entire system including caching.

int main() {
    // Initialize cache
    init_cache();

    // Create two messages
    Message* msg1 = create_msg(1, "Shorena", "Mike", "Good morning, Mike!", 0);
    Message* msg2 = create_msg(2, "Cheng", "Nino", "Hello, Nino!", 0);

    // Store both messages
    if (msg1) {
        store_msg(msg1);
        free_msg(msg1);
    }

    if (msg2) {
        store_msg(msg2);
        free_msg(msg2);
    }

    // First retrieval (will load from disk, store in cache)
    Message* retrieved_msg1 = retrieve_msg(1);
    printf("\nFirst retrieval of message ID 1:\n");
    if (retrieved_msg1) 
        print_msg(retrieved_msg1);

    // Second retrieval (should hit the cache)
    Message* retrieved_msg1_again = retrieve_msg(1);
    printf("\nSecond retrieval of message ID 1 (should be from cache):\n");
    if (retrieved_msg1_again) 
        print_msg(retrieved_msg1_again);

    return 0;
}
