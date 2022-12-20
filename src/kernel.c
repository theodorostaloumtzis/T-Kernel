# include "kernel.h"
#include <stddef.h>
#include <stdint.h>


uint16_t* vidptr = 0;  
uint16_t terminal_row = 0;
uint16_t terminal_column = 0;

uint16_t terminal_make_char(char c, char colour){
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour){
    vidptr[y * VGA_WIDTH + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour){
    if(c== '\n'){
        terminal_column = 0;
        terminal_row++;
        return;
    }
    terminal_putchar(terminal_column, terminal_row, c, colour);
    terminal_column++;
    if(terminal_column >= VGA_WIDTH){
        terminal_column = 0;
        terminal_row++;
    }
}

void terminal_initialize(){
    vidptr = (uint16_t*)0xB8000;
    terminal_row = 0;
    terminal_column = 0;
    for(int y = 0; y < VGA_HEIGHT; y++){
        for(int x = 0; x < VGA_WIDTH; x++){
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

size_t strlen(const char* str){
    size_t len = 0;
    while(str[len])
        len++;
    return len;terminal_writechar('A', 15);
    terminal_writechar('B', 15);
}

void print(const char* str){
    for(size_t i = 0; i < strlen(str); i++){
        terminal_writechar(str[i], 15);
    }
}


void kernel_main(){

    terminal_initialize();

    print("Hello, world! \n test \n test2");
    
}