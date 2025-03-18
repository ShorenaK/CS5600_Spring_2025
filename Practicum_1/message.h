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

// the Message structure
// This structure represents a message with necessary fields: ID, time sent, sender, receiver, content, and delivered status.
//
typedef struct {
    int id;               // Unique identifier
    time_t time_sent;     // Time sent
    char sender[100];     // Sender (text)
    char receiver[100];   // Receiver (text)
    char content[1000];   // Content of the message
    int delivered;        // Flag indicating delivery status
} Message;
// Function prototype
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);
int store_msg(const Message* msg);
Message* retrieve_msg(int id);

#endif // MESSAGE_H

