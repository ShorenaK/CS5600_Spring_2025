#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "pqueuepmain.h"

/*
* pqueuepmain.c / Practice Assignment / Complex Synchronization
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Feb , 2025
*
*/

// Buffer to store produced items, with a fixed size defined by BUF_SIZE
int buffer[BUF_SIZE];

// Variable to keep track of the number of items currently in the buffer
int num = 0;

// Counter to track the total number of items produced
int total_produced = 0;

// Counter to track the total number of items consumed
int total_consumed = 0;

// Mutex to control access to shared resources, preventing race conditions
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

// Condition variable to synchronize producer and consumer threads
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]) {
    (void)argc;  
    (void)argv; 

    // Two producer and two consumer threads
    pthread_t producers[2], consumers[2];  

    // Initialize mutex and condition variable
    if(pthread_mutex_init(&mut, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if(pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);
    }

    // Create producer threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&producers[i], NULL, producer, NULL) != 0) {
            perror("pthread_create producer");
            exit(1);
        }
    }

    // Create consumer threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            perror("pthread_create consumer");
            exit(1);
        }
    }

    /* Let the threads run for 30 seconds */
    sleep(30);
    printf("WE ARE DONE\n");

    // Cleanup threads
    for (int i = 0; i < 2; i++) {
        pthread_cancel(producers[i]);
        pthread_cancel(consumers[i]);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    return 0;
}

// Producer thread function
void *producer(void *param) {
    (void)param;
    int i = 0;
    while (1) {
	// Lock before accessing shared resource
        pthread_mutex_lock(&mut); 
        // Wait if the buffer is full
        while (num == BUF_SIZE) {
            pthread_cond_wait(&cond, &mut);
        }

        // Produce an item and add it to the buffer
        buffer[num] = i++;
        num++;
        total_produced++;
        printf("Produced: %d (Buffer size: %d) | Total Produced: %d\n", buffer[num - 1], num, total_produced);

	// Unlock after modifying the buffer
        pthread_mutex_unlock(&mut);  
	pthread_cond_signal(&cond);  

        // Exit once total production reaches the target
        if (total_produced >= TOTAL_ITEMS) {
            printf("Produced 30 items. Exiting producer.\n");
            pthread_exit(NULL);
        }

        sleep(1);  
    }
}

// Consumer thread function
void *consumer(void *param) {
    (void)param;
    int item;
    while (1) {
	// Lock before accessing shared resource
        pthread_mutex_lock(&mut);  

        // Wait if the buffer is empty
        while (num == 0) {
            pthread_cond_wait(&cond, &mut);
        }

        // Consume an item from the buffer
        item = buffer[num - 1];
        num--;
        total_consumed++;
        printf("Consumed: %d (Buffer size: %d) | Total Consumed: %d\n", item, num, total_consumed);

	// Unlock after consuming item
        pthread_mutex_unlock(&mut);  
	pthread_cond_signal(&cond);  

        // Exit once total consumption reaches the target
        if (total_consumed >= TOTAL_ITEMS) {
            printf("Consumed 30 items. Exiting consumer.\n");
            pthread_exit(NULL);
        }

        sleep(3);  
    }
}
