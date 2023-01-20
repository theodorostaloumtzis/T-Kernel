#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"


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

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}



void print(const char* str)
{
    size_t len = strlen(str);
    for(size_t i = 0; i < len; i++)
    {
        terminal_write_char(str[i], 15);
    }
}



void kernel_main() 
{
    terminal_initialize();
    
    print("Hello, world!\ntest\ntest2\n");

    // Initialize the interrupt descriptor table
    idt_init();

}