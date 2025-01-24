CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Build all executables
all: allocmem loadmem

# Target to build allocmem
allocmem: allocmem.c
	$(CC) $(CFLAGS) -o allocmem allocmem.c

# Target to build loadmem
loadmem: loadmem.c loadmem.h
	$(CC) $(CFLAGS) -o loadmem loadmem.c

# Clean target to remove compiled executables
clean:
	rm -f allocmem loadmem

