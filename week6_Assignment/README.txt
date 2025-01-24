Multiprocessing in C Assignment

Overview :  This project generates random strings, stores them in a queue, and encrypts them using multiprocessing in C.

Files in the Project area following:

genrand.c - Generates random strings and writes them to a file.

queue.c - Handles queue operations.

queue.h - Header file for queue operations.

process_manager.c - Reads strings, stores them in a queue, and calls the cipher program.

cipher.c - Encrypts or decrypts strings using the Polybius cipher.

polybius.c - Implements the Polybius cipher logic.

polybius.h - Header file for the Polybius cipher functions.

Makefile - Compiles all project files.

To compile the project, we run:  make all

This will generate the executables: genrand , cipher, process_manager

Steps on how to Run the Program: 

Step 1: To Generate Random Strings, we Run the following command to generate 10,000 random strings and save them in strings.txt:

./genrand 10000 strings.txt

Expected Output:

Successfully generated 10000 random strings in overwrite mode. ---> This confirms that program is working. 

Step 2: To Process and Encrypt Strings, we run the process manager to read the strings and encrypt them in batches of 100:

./process_manager

Expected Output:

Words stored in queue:
DOJMjelsLm
FczGFgnuZU
HapUDmpcTS
RQrSZIDuxs
zONIvSpADC
ZIjyxwraZQ
neBcnSBIHW
BsnqdUXKVc
fOVwzaZIzy
REzalEjmhc
so forth .....

Processing complete. Encrypted files generated.  ---> This confirms that program is working. 

After running, we will see files like output_1.txt, output_2.txt, and so forth.

Step 3: To View an Encrypted Output File, we need to run: 

cat output_1.txt

Step 4: To Decrypt a String, we can test the decryption of a specific encoded string, by using:

echo "133432354544154200435443441532001331114343" | ./cipher -d Expected Output: COMPUTER SYSTEM CLASS

or 

echo "14342432241531433132" | ./cipher -d   Expected Output:   DOJMjelsLm

To encrypt we run: 

echo "DOJMjelsLm" | ./cipher -e  Expected Output: 14342432241531433132

or

echo "Computer System Class" | ./cipher -e  Expected Output: 133432354544154200435443441532001331114343

Step 5: Clean Up and remove Compiled Files we run:  make clean

