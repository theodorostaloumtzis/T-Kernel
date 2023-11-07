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

int memcmp(void* s1, void* s2, int count) // compare memory
{
    char* c_s1 = (char*)s1;
    char* c_s2 = (char*)s2;
    while(count-- >0) // while count is greater than 0
    {
        if(*c_s1++ != *c_s2++)  // if they are not equal return -1 if s1 is less than s2 and 1 if s1 is greater than s2
            return c_s1[-1] < c_s2[-1] ? -1 : 1;  // if they are not equal return -1 if s1 is less than s2 and 1 if s1 is greater than s2 (ternary operator
    }
    return 0;  // if they are equal

}