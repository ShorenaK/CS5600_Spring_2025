# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread

# Target to build all executables
all: pqueuepmain multithread

# Build the producer-consumer executable
pqueuepmain: pqueuepmain.c
	$(CC) $(CFLAGS) -o pqueuepmain pqueuepmain.c

# Build the multithreading example executable
multithread: multithread.c
	$(CC) $(CFLAGS) -o multithread multithread.c

# Clean target to remove compiled files
clean:
	rm -f pqueuepmain multithread
