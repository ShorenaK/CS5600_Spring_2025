#ifndef PQUEUEPMAIN_H
#define PQUEUEPMAIN_H

// Buffer capacity updated to 15 & Target production and consumption count
#define BUF_SIZE 15  
#define TOTAL_ITEMS 30  

// Mutex for critical section
pthread_mutex_t mut;  
// Condition variable for synchronization
pthread_cond_t cond;  


// Shared buffer and counters
extern int buffer[BUF_SIZE];
extern int num;
// Track total produced items
extern int total_produced;
// Track total consumed items
extern int total_consumed;

// Thread synchronization objects
extern pthread_mutex_t mut;
extern pthread_cond_t cond;

// Function prototypes
void *producer(void *param);
void *consumer(void *param);

#endif  // PQUEUEPMAIN_H
