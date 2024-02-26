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

int memcmp(void* s1, void* s2, int count)  // compare memory
{
    char* c1 = s1; 
    char* c2 = s1;
    while(count-- > 0){
        if(*c1++ != *c2++){
            return c1[-1] < c2[-1] ? -1 : 1; // if c1 is less than c2, return -1, else return 1
        }
    }
    
    return 0;
}