#include "memory.h"

void* memset(void* ptr, int value, size_t size){
    char* c_ptr = (char*) ptr;
    for(int i = 0; i < size; i++){
        c_ptr[i] = (char) c;
    }
}