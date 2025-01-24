#ifndef LOADMEM_H
#define LOADMEM_H

#define MAX_LINE_LENGTH 1024  

typedef struct {
    int *data;  
    int size;   
} dyn_block;

// Function prototypes
dyn_block* alloc_dyn_block(int size);
void store_mem_blk(dyn_block *block, int *data, int size);
void read_file_and_store_blocks(const char *filename);

#endif
