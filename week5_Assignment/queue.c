#include "queue.h"

/*
* queue.c / Practice Assignment / Program Execution and Process Queue
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Feb , 2025 
*
*/

// Function to add an element to the end of the queue
void push_queue(queue_t* queue, void* element) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = element;
    new_node->next = NULL;
    
    if (queue->tail == NULL) {
        queue->head = queue->tail = new_node;
        new_node->prev = NULL;
    } else {
        queue->tail->next = new_node;
        new_node->prev = queue->tail;
        queue->tail = new_node;
    }
    queue->size++;
}

// Function to remove and return the front element from the queue
void* pop_queue(queue_t* queue) {
    if (queue->head == NULL) return NULL;

    node_t* temp = queue->head;
    void* data = temp->data;
    queue->head = queue->head->next;

    if (queue->head == NULL)
        queue->tail = NULL;
    else
        queue->head->prev = NULL;

    free(temp);
    queue->size--;
    return data;
}

// Function to remove and return the process with the highest priority in the queue
process_t* remove_process(queue_t* queue) {
    if (queue->head == NULL) return NULL;

    node_t *current = queue->head, *highest_priority_node = queue->head;
    process_t *highest_priority_process = (process_t *)current->data;

    while (current) {
        process_t *proc = (process_t *)current->data;
        if (proc->priority > highest_priority_process->priority) {
            highest_priority_process = proc;
            highest_priority_node = current;
        }
        current = current->next;
    }

    if (highest_priority_node->prev)
        highest_priority_node->prev->next = highest_priority_node->next;
    else
        queue->head = highest_priority_node->next;

    if (highest_priority_node->next)
        highest_priority_node->next->prev = highest_priority_node->prev;
    else
        queue->tail = highest_priority_node->prev;

    free(highest_priority_node);
    queue->size--;
    return highest_priority_process;
}

// Function to get the number of elements in the queue
int get_queue_size(queue_t* queue) {
    return queue->size;
}
