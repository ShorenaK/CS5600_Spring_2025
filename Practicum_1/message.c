/*
* Practicum 1
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March , 2025
*
* file name message.c
*/
// message.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

Message* create_msg(int id, const char* sender, const char* receiver, const char* content) {
    Message* new_msg = (Message*)malloc(sizeof(Message));
    if (new_msg == NULL) {
        return NULL;
    }
    
    new_msg->id = id;
    new_msg->time_sent = time(NULL);  // Current time
    strncpy(new_msg->sender, sender, sizeof(new_msg->sender) - 1);
    new_msg->sender[sizeof(new_msg->sender) - 1] = '\0';  // Ensure null-termination
    strncpy(new_msg->receiver, receiver, sizeof(new_msg->receiver) - 1);
    new_msg->receiver[sizeof(new_msg->receiver) - 1] = '\0';
    strncpy(new_msg->content, content, sizeof(new_msg->content) - 1);
    new_msg->content[sizeof(new_msg->content) - 1] = '\0';
    new_msg->delivered = 0;

    return new_msg;
}

int store_msg(const Message* msg) {
    FILE* file = fopen("message_store.txt", "a");
    if (file == NULL) {
        return -1;
    }

    fprintf(file, "%d,%ld,%s,%s,%s,%d\n", msg->id, msg->time_sent, msg->sender, msg->receiver, msg->content, msg->delivered);
    fclose(file);
    return 0;
}

Message* retrieve_msg(int id) {
    FILE* file = fopen("message_store.txt", "r");
    if (file == NULL) {
        return NULL;
    }

    Message* msg = NULL;
    int msg_id;
    time_t time_sent;
    char sender[100], receiver[100], content[1000];
    int delivered;

    while (fscanf(file, "%d,%ld,%99[^,],%99[^,],%999[^,],%d\n", &msg_id, &time_sent, sender, receiver, content, &delivered) == 6) {
        if (msg_id == id) {
            msg = (Message*)malloc(sizeof(Message));
            if (msg == NULL) {
                fclose(file);
                return NULL;
            }

            msg->id = msg_id;
            msg->time_sent = time_sent;
            strncpy(msg->sender, sender, sizeof(msg->sender));
            strncpy(msg->receiver, receiver, sizeof(msg->receiver));
            strncpy(msg->content, content, sizeof(msg->content));
            msg->delivered = delivered;
            break;
        }
    }

    fclose(file);
    return msg;
}


