#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "message.h"
#include "storage.h"

/*
* main.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/

#define NUM_MESSAGES 20
#define NUM_ACCESSES 1000
// This file tests the entire system including caching.
int main() {
    // === Manual testing (Part 2) ===
    printf("=== Manual Test ===\n");
    // Reset cache
    init_cache();  
    // Start with LRU
    cache_replacement_policy = LRU_REPLACEMENT;  

    // Create and store two manual test messages
    Message* msg1 = create_msg(1, "Shorena", "Mike", "Good morning, Mike!", 0);
    Message* msg2 = create_msg(2, "Cheng", "Nino", "Hello, Nino!", 0);

    if (msg1) {
        store_msg(msg1);
        free_msg(msg1);
    }

    if (msg2) {
        store_msg(msg2);
        free_msg(msg2);
    }

    // Retrieve message ID 1 twice to test cache hit
    Message* retrieved1 = retrieve_msg(1);
    printf("\nFirst retrieval of message ID 1:\n");
    if (retrieved1) {
        print_msg(retrieved1);
        // This one came from disk
        free_msg(retrieved1);  
    }

    Message* retrieved1_again = retrieve_msg(1);
    printf("\nSecond retrieval of message ID 1 (should be from cache):\n");
    if (retrieved1_again) {
        // DO NOT free (from cache)
        print_msg(retrieved1_again);  
    }

    // === Evaluation Test (Part 3 and 4) ===
    printf("\n=== Evaluation Test ===\n");

    // Run both replacement strategies
    for (int policy = 0; policy < 2; policy++) {
        // Reset cache again
        init_cache();  

        if (policy == 0) {
            cache_replacement_policy = LRU_REPLACEMENT;
        } else {
            cache_replacement_policy = RANDOM_REPLACEMENT;
        }

        printf("\n>> Testing Replacement Policy: %s\n",
            cache_replacement_policy == LRU_REPLACEMENT ? "LRU" : "Random");

        // Generate and store a pool of messages
        for (int i = 1; i <= NUM_MESSAGES; i++) {
            char sender[50], receiver[50], content[256];
            snprintf(sender, sizeof(sender), "Sender%d", i);
            snprintf(receiver, sizeof(receiver), "Receiver%d", i);
            snprintf(content, sizeof(content), "Test message content %d", i);

            Message* msg = create_msg(i, sender, receiver, content, 0);
            if (msg) {
                store_msg(msg);
                free_msg(msg);
            }
        }

        // Simulate random access and track performance
        int hits = 0, misses = 0;

        for (int i = 0; i < NUM_ACCESSES; i++) {
            int random_id = (rand() % NUM_MESSAGES) + 1;
            Message* result = retrieve_msg(random_id);

            // Cache is global, so we can compare address range
            if (result >= cache && result < cache + CACHE_SIZE) {
                hits++;
            } else {
                misses++;
                free_msg(result);
            }
        }

        // Output results
        printf("Total Accesses: %d\n", NUM_ACCESSES);
        printf("Cache Hits: %d\n", hits);
        printf("Cache Misses: %d\n", misses);
        printf("Hit Ratio: %.2f%%\n", (hits * 100.0) / NUM_ACCESSES);
    }

    return 0;
}
