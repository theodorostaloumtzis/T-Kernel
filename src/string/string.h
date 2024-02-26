#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
int strlen(const char* ptr);
bool isdigit(char c);
int tonumericdigit(char c);
int strnlen(const char* ptr, int max);


#endif // STRING_H