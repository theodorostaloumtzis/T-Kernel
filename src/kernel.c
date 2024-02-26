#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "string/string.h"
#include "disk/disk.h"
#include "fs/pparser.h"


uint16_t* video_memory = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;



uint16_t terminal_make_char(char c, char color) 
{
    return (color << 8) | c;
}

void terminal_set_char( size_t x, size_t y, char c, char color)
{
    video_memory[y * VGA_WIDTH + x] = terminal_make_char(c, color);
}

void terminal_write_char(char c, char color)
{
    if(c == '\n'){
        terminal_col = 0;
        terminal_row++;
        return;
    }

    terminal_set_char(terminal_col, terminal_row, c, color);
    terminal_col++;
    if(terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
    
}

void terminal_initialize()
{
    video_memory = (uint16_t*)0xB8000;
    for(int y = 0; y < VGA_HEIGHT; y++)
    {
        for(int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_set_char(x, y, ' ', 0);
        }
    }
   
}


void print(const char* str)
{
    size_t len = strlen(str);
    for(size_t i = 0; i < len; i++)
    {
        terminal_write_char(str[i], 15);
    }
}


static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main() 
{
    terminal_initialize();
    
    print("Hello, world!\ntest\ntest2\n");

    // Initialize the kernel heap
    kheap_init();

    // Search for and initialize the disk
    disk_search_and_init();

    // Initialize the interrupt descriptor table
    idt_init();

    // Initialize paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE|PAGING_IS_PRESENT|PAGING_ACCESS_FROM_ALL);

    // Switch to the kernel directory
    paging_switch(paging_get_directory(kernel_chunk));

    char* ptr = kzalloc(4096); 
    paging_set(paging_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // Enable paging
    enable_paging();

    // Enable interrupts
    enable_interrupts();

    // Test the path parser
    struct path_root* root_path = pathparser_parse("0:/usr/bin/shell.exe", NULL);
    if(root_path){
        print("Path parsed successfully\n");
        pathparser_free(root_path);
    }
}
