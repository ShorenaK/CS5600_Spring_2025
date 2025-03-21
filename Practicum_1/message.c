#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

/*
* message.c / Practicum 1 
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/

/* Implemention of function for creating, printing, and freeing messages.
 *
 */

// Creates a new message dynamically
Message* create_msg(int id, const char* sender, const char* receiver, const char* content, int delivered) {
    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    msg->id = id;
    msg->time_sent = time(NULL);
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    strncpy(msg->receiver, receiver, sizeof(msg->receiver) - 1);
    strncpy(msg->content, content, sizeof(msg->content) - 1);
    msg->delivered = delivered;
    return msg;
}
// Part 2 ---> Caching




// Prints a message
void print_msg(Message* msg) {
    if (!msg) {
        printf("Message not found.\n");
        return;
    }
    printf("ID: %d\n", msg->id);
    printf("Sent Time: %s", ctime(&msg->time_sent));
    printf("Sender: %s\n", msg->sender);
    printf("Receiver: %s\n", msg->receiver);
    printf("Content: %s\n", msg->content);
    printf("Delivered: %d\n", msg->delivered);
}

// Frees allocated memory for a message
void free_msg(Message* msg) {
    if (msg) free(msg);
}
