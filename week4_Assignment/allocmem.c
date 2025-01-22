#include <stdio.h>
#include <stdlib.h>

// name: Shorena K. Anzhilov
// email: anzhilov.s@northeastern.com

#include <stdio.h>
#include <stdlib.h>

// Static memory allocation (5 MB)
static char static_memory[5 * 1024 * 1024]; 

int main() {
    // Stack memory allocation (1 MB)
    char stack_memory[1 * 1024 * 1024]; 

    // Heap memory allocation (10 MB)
    char *heap_memory = (char *)malloc(10 * 1024 * 1024);  

    // Check if heap allocation was successful
    if (heap_memory == NULL) {
        printf("Heap memory allocation failed!\n");
        return 1;  
    }

    // Using allocated memory (writing dummy data)
    for (int i = 0; i < (5 * 1024 * 1024); i++) {
        static_memory[i] = 'A';  
    }
    for (int i = 0; i < (1 * 1024 * 1024); i++) {
        stack_memory[i] = 'B';  
    }
    for (int i = 0; i < (10 * 1024 * 1024); i++) {
        heap_memory[i] = 'C';  
    }

    // Printing memory usage confirmation
    printf("Memory allocation successful.\n");

    // Demonstrating a small portion of allocated data for verification
    printf("Sample data from memory: Static[%c] Stack[%c] Heap[%c]\n",
           static_memory[0], stack_memory[0], heap_memory[0]);

    // Memory cleanup when program exits:
    // Heap memory is freed manually with free()
    // Stack memory is automatically freed when the function exits
    
    // Free the allocated heap memory
    free(heap_memory);
    printf("Heap memory freed.\n");
    

    return 0;
}


