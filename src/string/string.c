#include "string.h"

// Returns the length of the string
int strlen(const char* ptr){
    int i = 0;
    while(*ptr != 0){
        i++;
        ptr++;
    }
    return i;
}

// Returns the length of the string, but no more than max
int strnlen(const char* ptr, int max){
    int i =0;
    for (i = 0; i < max; i++){
        if (ptr[i] == 0){
            break;
        }
    }
    return i;
}

bool isdigit(char c){
    return c >= 48 && c <= 57;
}

// Character c must be digit.
int tonumericdigit(char c){
    return c - 48;
}