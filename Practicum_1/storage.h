#ifndef STORAGE_H
#define STORAGE_H

#include "message.h"

#define MSG_FILE "messages.dat"

/*
* storage.h / Practicum 1
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / March, 2025
*
*/


// Function declarations for storeing and retrieving messages.
int store_msg(Message* msg);
Message* retrieve_msg(int id);

#endif

