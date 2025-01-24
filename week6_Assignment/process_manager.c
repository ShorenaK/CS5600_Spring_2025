#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

/*
* process_maneger.c / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025, Feb 2025
*
*/

#define BATCH_SIZE 100
#define MAX_WORD_LENGTH 100

void processBatch(Queue *q, int batchNumber) {
    if (isEmpty(q)) {
        printf("Queue is empty, nothing to process.\n");
        return;
    }
    // Pipe
    int fd[2];  
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: Run cipher program
        close(fd[1]); 

        // Redirect stdin to read from the pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        // Create output file name
        char outputFile[50];
        snprintf(outputFile, sizeof(outputFile), "output_%d.txt", batchNumber);
        
        // Redirect stdout to the output file
        FILE *out = fopen(outputFile, "w");
        if (!out) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(fileno(out), STDOUT_FILENO);
        fclose(out);

        // Execute the cipher program (modify as necessary)
        execl("./cipher", "./cipher", "-e", NULL);

        // If execl fails
        perror("Error executing cipher");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: Send batch of 100 words to cipher
            // close reading
	    close(fd[0]);  

        for (int i = 0; i < BATCH_SIZE && !isEmpty(q); i++) {
            char *word = dequeue(q);
            write(fd[1], word, strlen(word));
            write(fd[1], "\n", 1);  
            free(word);
        }
	// Close writting
        close(fd[1]); 

        // Wait for child Process to be finished
	wait(NULL);  
    }
}

int main(void) {
    Queue *q = createQueue();

    // Read words from file and enqueue them
    FILE *file = fopen("strings.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", buffer) == 1) {
        enqueue(q, buffer);
    }
    fclose(file);

    // Debugging: Print queue content before processing
    printf("Words stored in queue:\n");
    printQueue(q);

    // Debugging: Dequeue a few elements to verify
    printf("Dequeuing first element: %s\n", dequeue(q));
    printf("Dequeuing second element: %s\n", dequeue(q));

    // Print remaining queue after dequeuing some elements
    printf("Queue after dequeuing two elements:\n");
    printQueue(q);

    // Process in batches of 100
    int batchNumber = 1;
    while (!isEmpty(q)) {
        processBatch(q, batchNumber++);
    }

    freeQueue(q);
    printf("Processing complete. Encrypted files generated.\n");

    return 0;
}

