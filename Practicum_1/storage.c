#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Cache structure (array-based)
Message cache[CACHE_SIZE];  // Fixed-size array for caching messages
int cache_count = 0;        // Number of messages in the cache

// Initialize cache (clear all entries)
void init_cache() {
    cache_count = 0;  // Reset cache
}

// Search for a message in the cache
Message* cache_lookup(int id) {
    for (int i = 0; i < cache_count; i++) {
        if (cache[i].id == id) {
            // Move the found message to the front (LRU strategy)
            Message temp = cache[i];
            for (int j = i; j > 0; j--) {
                cache[j] = cache[j - 1];
            }
            cache[0] = temp;
            return &cache[0];
        }
    }
    return NULL;  // Message not found in cache
}

// Insert a message into the cache
void cache_insert(Message* msg) {
    if (cache_count < CACHE_SIZE) {
        // Insert at the front
        for (int i = cache_count; i > 0; i--) {
            cache[i] = cache[i - 1];
        }
        cache[0] = *msg;
        cache_count++;
    } else {
        // Cache is full, remove the least recently used (LRU) entry
        for (int i = CACHE_SIZE - 1; i > 0; i--) {
            cache[i] = cache[i - 1];
        }
        cache[0] = *msg;
    }
}

// Stores a message in a file cache and disk 
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

    // store in cache
    cache_insert(msg);
    // Success
    return 0; 
}

// Retrieve a message by id and (check cache first, then disk)
Message* retrieve_msg(int id) {
    // Check the cache first
    Message* cached_msg = cache_lookup(id);
    if (cached_msg) {
        printf("Cache hit! Message retrieved from cache.\n");
        return cached_msg;
    }

    // If not in cache, read from disk
    FILE* file = fopen(MSG_FILE, "rb");
    if (!file) {
        printf("Error: Failed to open file for reading.\n");
        return NULL;
    }

    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    while (fread(msg, sizeof(Message), 1, file)) {
        if (msg->id == id) {
            fclose(file);
            // Store message in cache
            cache_insert(msg);
            return msg;
        }
    }

    fclose(file);
    free(msg);
    return NULL;  // Message not found
}
