#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include  <stdbool.h>


#define PAGING_CACHE_DISABLE    0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITEABLE     0b00000010
#define PAGING_IS_PRESENT       0b00000001


#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096


struct paging_4gb_chunk
{
    uint32_t* directory_entry;
};

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags); // Create a new 4GB chunk
void paging_switch(uint32_t* directory_entry); // Switch to a new directory
uint32_t* paging_get_directory(struct paging_4gb_chunk* chunk); // Get the directory entry of a chunk
bool paging_is_aligned(void* address); // Check if an address is aligned
int paging_get_indexes(uint32_t virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out); // Get the indexes of a virtual address
int paging_set(uint32_t* directory, void* virt, uint32_t val); // Set a value in a directory
void enable_paging(); // Enable paging

#endif // PAGING_H