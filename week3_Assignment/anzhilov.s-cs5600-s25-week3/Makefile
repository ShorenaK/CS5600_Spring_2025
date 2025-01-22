# Compiler to use 
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Target: Build the cipher executable
cipher: cipher.c polybius.c
	$(CC) $(CFLAGS) -o cipher cipher.c polybius.c

# Clean target to remove the executable
clean:
	rm -f cipher
