# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror

# Targets to build all executables
all: learnc arger

# Build the learnc executable
learnc: learnc.c mpg2km.c
	$(CC) $(CFLAGS) -o learnc learnc.c mpg2km.c

# Build the arger executable 
arger: arger.c
	$(CC) $(CFLAGS) -o arger arger.c

# Clean up compiled files
clean:
	rm -f learnc arger

