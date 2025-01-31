/*
* polybius.h / Practice Assignment / Multiprocessing in C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025, Feb , 2025
*
*/

#ifndef POLYBIUS_H
#define POLYBIUS_H

// Header file for Polybius cipher functions.
// It contains function prototypes for encoding and decoding
// using the Polybius square cipher.

// Function prototypes
void pbEncode(const char *plaintext, char *encodedText);
void pbDecode(const char *ciphertext, char *decodedText);

#endif  

