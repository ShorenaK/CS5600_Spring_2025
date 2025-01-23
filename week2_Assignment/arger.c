/*
 * arger.c / Practice Assignment / C  
 *
 * Shorena K. Anzhilov / CS5600 / Northeastern University
 * Spring 2025 / Jan, 2025
 *
 * This program processes command-line arguments to convert text based on options.
 */

#include <stdio.h>

// Function prototypes
void to_uppercase(char *str);
void to_lowercase(char *str);
void to_capitalize(char *str);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-u|-l|-cap] <text>\n", argv[0]);
        return -1;  
    }

    char *option = argv[1];

    // Concatenating all arguments into one text string
    for (int i = 2; i < argc; i++) {
        if (i > 2) {
            printf(" ");
        }

        if (option[0] == '-' && option[1] == 'u' && option[2] == '\0') {
            to_uppercase(argv[i]);
        } 
        else if (option[0] == '-' && option[1] == 'l' && option[2] == '\0') {
            to_lowercase(argv[i]);
        } 
        else if (option[0] == '-' && option[1] == 'c' && option[2] == 'a' && option[3] == 'p' && option[4] == '\0') {
            to_capitalize(argv[i]);
        } 
        else {
            printf("Error: Invalid option. Use -u, -l, or -cap\n");
            return -1;  
        }
    }

    printf("\n");
    return 0;
}

// Function to convert a string to uppercase
void to_uppercase(char *str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32;  
        }
        putchar(*str);
        str++;
    }
}

// Function to convert a string to lowercase
void to_lowercase(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 32;  
        }
        putchar(*str);
        str++;
    }
}

// Function to capitalize the first letter of each word
void to_capitalize(char *str) {
    int capitalize_next = 1;
    while (*str) {
        if (*str == ' ') {
            capitalize_next = 1;
            putchar(*str);
        } else if (capitalize_next && *str >= 'a' && *str <= 'z') {
            putchar(*str - 32);  
            capitalize_next = 0;
        } else {
            if (*str >= 'A' && *str <= 'Z')
                *str += 32;  
            putchar(*str);
            capitalize_next = 0;
        }
        str++;
    }
}
