#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
* pqueuepmain.c / Practice Assignment / Complex Synchronization
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Feb , 2025
*
*/


// Buffer capacity updated to 15 & Target production and consumption count

#define BUF_SIZE 15  
#define TOTAL_ITEMS 30  

// Shared buffer and counters
int buffer[BUF_SIZE];
int num = 0; 
// Track total produced items
int total_produced = 0;  
// Track total consumed items
int total_consumed = 0;  

// Mutex for critical section
pthread_mutex_t mut;  
// Condition variable for synchronization
pthread_cond_t cond;  

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]) {
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
