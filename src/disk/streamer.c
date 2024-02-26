#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct  disk_stream* disk_streamer_new(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if(!disk){
        return 0;
    }

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;

}

int diskstreamer_seek(struct disk_stream* stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total)
{
    int sector = stream->pos / T_KERNEL_SECTOR_SIZE;
    int offset = stream->pos % T_KERNEL_SECTOR_SIZE;
    char buffer[T_KERNEL_SECTOR_SIZE];

    int res = disk_read_block(stream->disk, sector, 1, buffer);
    if(res < 0){
        goto out;
    }

    int total_to_read = total > T_KERNEL_SECTOR_SIZE ? T_KERNEL_SECTOR_SIZE : total;
    for(int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buffer[offset+i]; 
    }


    //Adjust the stream
    stream->pos += total_to_read;
    if(total > T_KERNEL_SECTOR_SIZE){
        res = diskstreamer_read(stream, out, total - T_KERNEL_SECTOR_SIZE);
    }
out:
    return res;
}

void diskstreamer_close(struct disk_stream* stream)
{
    kfree(stream);
}