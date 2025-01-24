README - Multiprocessing in C Assignment

Overview

This project is about generating random strings, storing them in a queue, and encrypting them using multiprocessing in C.

Files in the Project

genrand.c - Generates random strings and writes them to a file.

queue.c - Handles queue operations.

queue.h - Header file for queue operations.

process_manager.c - Reads strings, stores them in a queue, and calls the cipher program.

cipher.c - Encrypts or decrypts strings using the Polybius cipher.

polybius.c - Implements the Polybius cipher logic.

polybius.h - Header file for the Polybius cipher functions.

Makefile - Compiles all project files.

To compile the project, run:   make all

This will generate the executables: genrand cipher process_manager

Step 1: Generate Random Strings

We need to Run the following command to generate 10,000 random strings and save them in strings.txt:

./genrand 10000 strings.txt

Step 2: Process and Encrypt Strings we need to Run the process manager to read the strings and encrypt them in batches of 100:

./process_manager , After running, we will see files like output_1.txt, output_2.txt, so forth.

Step 3: To view an encrypted output file: we will use cat output_1.txt

Step 4: To Decrypt a String we can test the decryption of a specific encoded string using:

echo "1231231234" | ./cipher -d

Step 5: To remove the compiled files, we run: make clean
