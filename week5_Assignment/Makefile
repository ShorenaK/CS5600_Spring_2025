CC = gcc  # Compiler to use
CFLAGS = -Wall -Werror -g  # Compiler flags for warnings and debugging

# Default target to build the executable
all: queuetest

# Rule to build the queuetest executable from object files
queuetest: queue.o queuetest.o
	$(CC) $(CFLAGS) -o queuetest queue.o queuetest.o

# Rule to compile queue.c into an object file
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

# Rule to compile queuetest.c into an object file
queuetest.o: queuetest.c queue.h
	$(CC) $(CFLAGS) -c queuetest.c

# Clean up compiled files
clean:
	rm -f *.o queuetest

