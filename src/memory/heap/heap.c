#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>


static int heap_table_validate(void* ptr, void* end, struct heap_table* table) // validate table
{
    int res = 0;
    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / PERSONALOS_HEAP_BLOCK_SIZE;
    if(table->total != total_blocks)
    {
        res = -EINVARG;
        goto out;
    } 


out:
    return res;      
}

static bool heap_validate_alignment(void* ptr) // validate alignment
{
    return ((unsigned int)ptr % PERSONALOS_HEAP_BLOCK_SIZE) == 0;
}
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table) // create heap
{
    int res = 0; // result

    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0 ,sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heap_table_validate(ptr, end, table);
    if(res < 0)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY)* table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENRTY_FREE, table_size);
out:    
    return res;
}

static uint32_t heap_align_value_to_upper(uint32_t val) // align value to upper
{
    if(val % PERSONALOS_HEAP_BLOCK_SIZE == 0) // if value is already aligned
    {
        return val; // return value
    }
    val = (val - (val % PERSONALOS_HEAP_BLOCK_SIZE)); // align value
    val += PERSONALOS_HEAP_BLOCK_SIZE; // add block size
    return val; // return value
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry) // get entry type
{
    return entry & 0x0f; // return entry type
}

int heap_get_start_block(struct heap* heap, uint32_t total_blocks) // get start block
{
    struct heap_table* table = heap->table; // table
    int bc = 0; // block count
    int bs = -1; // block start

    for(size_t i = 0; i<table->total; i++ )
    {
        if(heap_get_entry_type(table->entries[i] != HEAP_BLOCK_TABLE_ENRTY_FREE))  // if entry is not free
        {
            bc = 0;  // block count is 0
            bs = -1; // block start is -1
            continue; 
        }

        if(bs == -1)  // if this is the first block
        {
            bs = i;  // block start is i
        }
        bc++;  // increment block count

        if(bc == total_blocks)  // if block count is equal to total blocks
        {
            break;  // break
        }
    }

    if(bs == -1)  // if block start is -1
    {
        return -ENOMEM;  // return no memory
    }
    return bs;
}

void* heap_block_to_address(struct heap* heap, int block)  // get address from block
{
    return (heap->saddr + (block * PERSONALOS_HEAP_BLOCK_SIZE));  // return address
}

void heap_mark_blocks_taken(struct heap* heap, int start_block, uint32_t total_blocks)  // mark blocks as taken
{
    int end_block = (start_block + total_blocks) - 1;  // end block index
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENRTY_TAKEN | HEAP_BLOCK_IS_FIRST;  // entry
    if(total_blocks > 1)  // if total blocks is greater than 1
    {
        entry |= HEAP_BLOCK_HAS_NEXT;  // entry has next
    }

    for(int i = start_block; i <= end_block; i++)  // loop through blocks
    {
        heap->table->entries[i] = entry;  // set entry
        entry = HEAP_BLOCK_TABLE_ENRTY_TAKEN;  // set entry to taken
        if(i != end_block - 1) // if this is not the last block
        {
            entry |= HEAP_BLOCK_HAS_NEXT;  // entry has next
        }
    }
    
}

void* heap_malloc_blocks(struct heap* heap , uint32_t total_blocks)  // malloc blocks
{
    void* address = 0;  // address
    int start_block = heap_get_start_block(heap, total_blocks);  // get start block

    if(start_block < 0)  // if start block is less than 0
    {
        goto out;
    }

    address = heap_block_to_address(heap, start_block);  // get address from block

    heap_mark_blocks_taken(heap, start_block, total_blocks);  // mark blocks as taken

out:
    return address;
}

void heap_mark_blocks_free(struct heap* heap, int start_block)  // mark blocks as free
{
    struct heap_table* table = heap->table;  // table
    for(int i = start_block; i < (int)table->total; i++)
    {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];  // entry
        table->entries[i] = HEAP_BLOCK_TABLE_ENRTY_FREE;  // set entry to free
        if(!(entry & HEAP_BLOCK_HAS_NEXT))  // if entry does not have next
        {
            break;  // break
        }
    }
}

int heap_address_to_block(struct heap* heap, void* address)  // get block from address
{
    return ((int)(address - heap->saddr) / PERSONALOS_HEAP_BLOCK_SIZE);  // return block
}

void* heap_malloc(struct heap* heap, size_t size)  // malloc
{
    size_t aligned_size = heap_align_value_to_upper(size);  // align size to upper
    uint32_t total_blocks = aligned_size / PERSONALOS_HEAP_BLOCK_SIZE;  // get total blocks
    return heap_malloc_blocks(heap, total_blocks);  // malloc blocks
}

void heap_free(struct heap* heap, void* ptr)  // free
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));  // mark blocks as free
}