#ifndef CONFIG_H
#define CONFIG_H


#define KERNEL_CODE_SELECTOR 0x08  // 0x08 is the kernel code selector

#define KERNEL_DATA_SELECTOR 0x10  // 0x10 is the kernel data selector

#define PERSONALOS_TOTAL_INTERRUPTS 512  // total number of interrupts

#define PERSONALOS_HEAP_SIZE_BYTES 1024*1024*100  // 100 MB heap size

#define PERSONALOS_HEAP_BLOCK_SIZE 4096 // 4 KB block size

#define PERSONALOS_HEAP_ADDRESS 0x01000000 // 16 MB heap address

#define PERSONALOS_HEAP_TABLE_ADDRESS 0x00007e00 // 2 KB heap table address

#define PERSONALOS_SECTOR_SIZE 512 // 512 bytes per sector


#endif