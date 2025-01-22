#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// name: Shorena K. Anzhilov
// email: anzhilov.s@northeastern.edu
// Spring, 2025

#ifndef QUEUE_H
#define QUEUE_H

// Define process struct
typedef struct {
    int id;
    char *name;
    long runtime;
    int priority;
} process_t;

// Node struct for the doubly linked list
typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node_t;

// Queue struct to hold elements
typedef struct {
    node_t *head;
    node_t *tail;
    int size;
} queue_t;

// Function to add an element to the end of the queue
void push_queue(queue_t* queue, void* element);

// Function to remove and return the front element from the queue
void* pop_queue(queue_t* queue);

// Function to remove and return the process with the highest priority in the queue
process_t* remove_process(queue_t* queue);

// Function to get the number of elements in the queue
int get_queue_size(queue_t* queue);

#endif 
