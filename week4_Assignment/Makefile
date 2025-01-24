# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Build all executables
all: allocmem loadmem

# Compile object files first
allocmem.o: allocmem.c
	$(CC) $(CFLAGS) -c allocmem.c -o allocmem.o

loadmem.o: loadmem.c loadmem.h
	$(CC) $(CFLAGS) -c loadmem.c -o loadmem.o

# Target to build allocmem
allocmem: allocmem.o
	$(CC) $(CFLAGS) -o allocmem allocmem.o

# Target to build loadmem
loadmem: loadmem.o
	$(CC) $(CFLAGS) -o loadmem loadmem.o

# Clean target to remove compiled executables and object files
clean:
	rm -f allocmem loadmem allocmem.o loadmem.o

