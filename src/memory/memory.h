#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void* memset(void* ptr, int c, size_t s); // set memory
int memcmp(void* s1, void* s2, int count); // compare memory

#endif