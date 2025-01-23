#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
* queuetest.c / Practice Assignment / Program Execution and Process Queue
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan , 2025 
*
*/

// Main function to test the queue operations
int main() {
    // Initialize an empty queue
    queue_t myQueue = {NULL, NULL, 0};

    // Create process instances with different priorities
    process_t p1 = {1, "Process1", 1000, 2};
    process_t p2 = {2, "Process2", 2000, 1};
    process_t p3 = {3, "Process3", 1500, 3};

    // Add processes to the queue
    printf("Adding processes to the queue...\n");
    push_queue(&myQueue, &p1);
    push_queue(&myQueue, &p2);
    push_queue(&myQueue, &p3);

    // Display the queue size after additions
    printf("Queue size after additions: %d\n", get_queue_size(&myQueue));

    // Remove the process with the highest priority and print it
    process_t *highest_priority = remove_process(&myQueue);
    printf("Removed highest priority process: %s\n", highest_priority->name);

    // Remove the front process and print it
    process_t *popped_process = pop_queue(&myQueue);
    printf("Popped process: %s\n", popped_process->name);

    // Print the final size of the queue
    printf("Final queue size: %d\n", get_queue_size(&myQueue));

    return 0;
}
