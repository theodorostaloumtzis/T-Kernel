#include "io/io.h"
#include "disk.h"
#include "memory/memory.h"
#include "config.h"
#include "status.h"

struct disk disk;

int disk_read_sector(int lba, int total, void* buf)
{
    outb(0x1F6, (lba >> 24) | 0xE0); // 0xE0 is the master drive
    outb(0x1F2, total); // Total sectors
    outb(0x1F3, (unsigned char)lba & 0xFF); // LBA low
    outb(0x1F4, (unsigned char)(lba >> 8)); // LBA mid
    outb(0x1F5, (unsigned char)(lba >> 16)); // LBA high
    outb(0x1F7, 0x20); // Command 0x20 - Read sectors

    unsigned short* ptr = (unsigned short*)buf;
    for(int b = 0; b<total; b++)
    {
        // Wait for the drive to be ready
        char c = insb(0x1F7);
        while(!(c & 0x08)) // Bit 3 is set when the drive is ready
        {
            c = insb(0x1F7);
        }
    }

    // Copy from hrddisk to memory
    for(int i=0; i<256; i++)
    {
        *ptr = insw(0x1F0);
        ptr++;
    }
 
    return 0;
}

void disk_search_init()
{
    memset(&disk , 0 , sizeof(disk));
    disk.type = PERSONALOS_DISK_TYPE_REAL;
    disk.sector_size = PERSONALOS_SECTOR_SIZE;
}

struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;

    return &disk;
}

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf)
{
    if(idisk != &disk)
        return -EIO; // Invalid argument

    return disk_read_sector(lba, total, buf);
}