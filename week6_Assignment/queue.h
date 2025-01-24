/*
* queue.h / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025, Feb 2025.
*
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a queue node
typedef struct Node {
    char *data;
    struct Node *next;
} Node;

// Define the queue structure
typedef struct {
    Node *front;
    Node *rear;
    int size;
} Queue;

// Function prototypes
Queue* createQueue(void);
void enqueue(Queue *q, const char *word);
char* dequeue(Queue *q);
int isEmpty(Queue *q);
void freeQueue(Queue *q);
void printQueue(Queue *q);

#endif 
