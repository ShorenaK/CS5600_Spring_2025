#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

/*
* message.h / Practicum 1
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March , 2025
*
*/

/* Declares the Message structure and functions relaed to message handling.
* This structure represents a message with necessary fields: ID, time sent, sender, receiver, content, and delivered status.
*/

// Defines a messagen structure
typedef struct {
    int id;               
    time_t time_sent;     
    char sender[100];     
    char receiver[100];   
    char content[1000];   
    int delivered;        
} Message;

// Function declarations
Message* create_msg(int id, const char* sender, const char* receiver, const char* content, int delivered);
void print_msg(Message* msg);
void free_msg(Message* msg);

#endif
