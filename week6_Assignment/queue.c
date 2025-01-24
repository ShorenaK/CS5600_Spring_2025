/*
* queue.c / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025
*
*/


/* queue.c - Implements queue functions */
#include "queue.h"

// Function to create a new queue
Queue* createQueue(void) {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        fprintf(stderr, "Error: Memory allocation failed for queue\n");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// Function to enqueue a word into the queue
void enqueue(Queue *q, const char *word) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed for new node\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = strdup(word);
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

// Function to dequeue a word from the queue
char* dequeue(Queue *q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Error: Queue underflow, no elements to dequeue\n");
        return NULL;
    }
    
    Node *temp = q->front;
    char *data = strdup(temp->data);
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp->data);
    free(temp);
    q->size--;

    return data;
}

// Function to check if the queue is empty
int isEmpty(Queue *q) {
    return (q->front == NULL);
}

// Function to free the queue and its memory
void freeQueue(Queue *q) {
    while (!isEmpty(q)) {
        free(dequeue(q));
    }
    free(q);
}

// Function to print the contents of the queue (for debugging purposes)
void printQueue(Queue *q) {
    Node *temp = q->front;
    while (temp != NULL) {
        printf("%s\n", temp->data);
        temp = temp->next;
    }
}
