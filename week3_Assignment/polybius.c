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

// Polybius square definition (excluding 'J', replaced with 'I')
char polybiusSquare[5][5] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},  
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
};

/*
 * Function: pbEncode
 * Encodes a plaintext message using the Polybius square cipher.
 * Spaces are encoded as '00'.
 *
 * plaintext: The input string to be encoded.
 * encodedText: The resulting encoded string.
 */
void pbEncode(const char *plaintext, char *encodedText) {
    int length = strlen(plaintext);
    int index = 0;

    for (int i = 0; i < length; i++) {
        char ch = toupper(plaintext[i]);
        if (ch == 'J') ch = 'I';  

        if (ch == ' ') {
            // Encode spaces as '00'
            encodedText[index++] = '0';
            encodedText[index++] = '0';
        } else if (ch >= 'A' && ch <= 'Z') {
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
 * Decodes a ciphertext message using the Polybius square cipher.
 * Decodes '00' as spaces.
 *
 * ciphertext: The input string to be decoded.
 * decodedText: The resulting decoded string.
 */
void pbDecode(const char *ciphertext, char *decodedText) {
    int length = strlen(ciphertext);
    int index = 0;

    for (int i = 0; i < length; i++) {
        if (ciphertext[i] == '0' && ciphertext[i + 1] == '0') {
            // Restore space
            decodedText[index++] = ' ';  
            i++;  
        } else if (isdigit(ciphertext[i]) && isdigit(ciphertext[i + 1])) {
            int row = ciphertext[i] - '1';
            int col = ciphertext[i + 1] - '1';
            decodedText[index++] = polybiusSquare[row][col];
            i++;  
        }
    }
    decodedText[index] = '\0';
}
