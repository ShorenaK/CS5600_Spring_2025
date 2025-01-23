# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Target to build all executables
all: cipher genrand

# Target to build the cipher executable
cipher: cipher.c polybius.c polybius.h
	$(CC) $(CFLAGS) -o cipher cipher.c polybius.c

# Target to build the genrand executable
genrand: genrand.c
	$(CC) $(CFLAGS) -o genrand genrand.c

# Clean target to remove executables
clean:
	rm -f cipher genrand

