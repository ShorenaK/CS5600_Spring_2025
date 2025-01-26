#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
* queuetest.c / Practice Assignment / Complex Synchronization
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Feb , 2025
*
*/

// Function that will run in a thread
void *print_message(void *arg) {
    // Print the message received from the main thread
    printf("Thread message: %s\n", (char *)arg);
    pthread_exit(NULL); 
}

int main() {
    pthread_t thread1, thread2;  
    char *msg1 = "Hello from Thread 1";
    char *msg2 = "Hello from Thread 2";

    // Create first thread
    if (pthread_create(&thread1, NULL, print_message, (void *)msg1) != 0) {
        printf("Error creating thread 1\n");
        return 1;  
    }

    // Create second thread
    if (pthread_create(&thread2, NULL, print_message, (void *)msg2) != 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Threads finished\n");
    return 0;
}
