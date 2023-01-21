#include "memory.h"

void* memset(void* ptr, int c, size_t size)  // set memory
{

    char* c_ptr = (char*)ptr;  // cast to char pointer
    for(int i = 0; i < size; i++)
    {  
        c_ptr[i] = c;  
    }
    return ptr;  
}