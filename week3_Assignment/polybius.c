#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

/*
* polybius.c / Practice Assignment / Random Number Generation and Encryption
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan , 2025 
*
*/

// Polybius square definition
char polybiusSquare[5][5] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},  
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
};

/*
 * Function: pbEncode
 * --- *** --- ***
 * Encodes a plaintext message using the Polybius square cipher.
 * --- *** --- *** 
 * plaintext: The input string to be encoded.
 * encodedText: The resulting encoded string.
 * --- *** --- ***
 * Special Characters Handling: Spaces and punctuation are ignored.
 */
void pbEncode(const char *plaintext, char *encodedText) {
    int length = strlen(plaintext);
    int index = 0;

    for (int i = 0; i < length; i++) {
        char ch = toupper(plaintext[i]);
        if (ch == 'J') ch = 'I';  // Convert 'J' to 'I'

        if (ch >= 'A' && ch <= 'Z') {
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    if (polybiusSquare[row][col] == ch) {
                        encodedText[index++] = '1' + row;
                        encodedText[index++] = '1' + col;
                        break;
                    }
                }
            }
        }
    }
    encodedText[index] = '\0';
}

/*
 * Function: pbDecode
 * --- *** --- ***
 * Decodes a ciphertext message using the Polybius square cipher.
 * --- *** --- ***
 * ciphertext: The input string to be decoded.
 * decodedText: The resulting decoded string in uppercase.
 */
void pbDecode(const char *ciphertext, char *decodedText) {
    int length = strlen(ciphertext);
    int index = 0;

    for (int i = 0; i < length; i++) {
        if (ciphertext[i] == '0') {
            // Restore space
	    decodedText[index++] = ' ';  
        } else if (isdigit(ciphertext[i]) && isdigit(ciphertext[i+1])) {
            int row = ciphertext[i] - '1';
            int col = ciphertext[i+1] - '1';
            decodedText[index++] = polybiusSquare[row][col];
            // Skip the next digit since it's part of the same letter
	    i++;  
        }
    }
    decodedText[index] = '\0';
}
