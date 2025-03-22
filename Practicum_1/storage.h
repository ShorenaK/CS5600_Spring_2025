#ifndef STORAGE_H
#define STORAGE_H

#include "message.h"

#define MSG_FILE "messages.dat"
// Number of messages stored in cache, that can store 16 messages
#define CACHE_SIZE 16 

/*
* storage.h / Practicum 1
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring,  March, 2025
*
*/

// Cache Replacement Policies
typedef enum {
    RANDOM_REPLACEMENT,
    LRU_REPLACEMENT
} CacheReplacementPolicy;

extern CacheReplacementPolicy cache_replacement_policy;  // Global variable for policy

// Function declarations for storeing and retrieving messages.
int store_msg(Message* msg);
Message* retrieve_msg(int id);

// Cache functions, intializes the cache, searchines for a message in the cache,
// and adds a new message to the cache.
void init_cache();
Message* cache_lookup(int id);
void cache_insert(Message* msg);

#endif
