# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Target to build all executables
all: genrand cipher process_manager

# Build the random string generator
genrand: genrand.c
	$(CC) $(CFLAGS) -o genrand genrand.c

# Build the cipher executable
cipher: cipher.c polybius.c polybius.h
	$(CC) $(CFLAGS) -o cipher cipher.c polybius.c

# Build the process manager for encryption
process_manager: process_manager.c queue.c queue.h
	$(CC) $(CFLAGS) -o process_manager process_manager.c queue.c

# Clean target to remove compiled files
clean:
	rm -f genrand cipher process_manager

