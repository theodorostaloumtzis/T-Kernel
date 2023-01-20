#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENRTY_TAKEN 0x01  // block is taken
#define HEAP_BLOCK_TABLE_ENRTY_FREE 0x00   // block is free

#define HEAP_BLOCK_HAS_NEXT  0b10000000   // block has next block
#define HEAP_BLOCK_IS_FIRST  0b01000000  // block has first block

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_table  // table of blocks
{
    HEAP_BLOCK_TABLE_ENTRY* entries;  // table of entries
    size_t total; // total number of entries

};

struct heap // heap
{
    struct heap_table* table; // table of blocks
    void* saddr;    // start address
};

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap, void* ptr);



#endif 