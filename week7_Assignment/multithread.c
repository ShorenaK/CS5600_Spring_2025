#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
* queuetest.c / Practice Assignment / Complex Synchronization
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Feb , 2025
*
*/

#define SIZE 5  // Define the size of the shared array

// Global shared data array
int shared_data[SIZE];

// Function for the writing thread
void *writer_thread(void *arg) {
    (void)arg;

    printf("Writer thread is adding data...\n");
    for (int i = 0; i < SIZE; i++) {
        // Write different values into the array
        shared_data[i] = i * 10; 
        // Simulate processing time
        usleep(100000);  
        printf("Writer added: %d at index %d\n", shared_data[i], i);
    }
    printf("Writer thread finished\n");
    return NULL;
}

// Function for the reading thread
void *reader_thread(void *arg) {
      (void)arg;

       printf("Reader thread is reading data...\n");
    for (int i = 0; i < SIZE; i++) {
        // Simulate delay to observe possible inconsistencies
        usleep(150000);  
        printf("Reader read: %d from index %d\n", shared_data[i], i);
    }
    printf("Reader thread finished\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
	
    pthread_t writer, reader;

    // Step 1: Create the writer thread
    if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
        printf("Error creating writer thread\n");
        return 1;
    }

    // Step 1: Create the reader thread
    if (pthread_create(&reader, NULL, reader_thread, NULL) != 0) {
        printf("Error creating reader thread\n");
        return 1;
    }
    // Let threads run for a while
    sleep(1);  

    // Step 2: Cancel the reader thread and print its ID
    printf("====> Cancelling Reader Thread (ID: %p)!!\n", (void *)reader);
    pthread_cancel(reader);

    // Small delay for message ordering
    usleep(100000);  

    // Step 2: Cancel the writer thread and print its ID
    printf("====> Cancelling Writer Thread (ID: %p)!!\n", (void *)writer);
    pthread_cancel(writer);

    // Step 1: Wait for both threads to complete
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    printf("Exiting from main program\n");
    return 0;
}
