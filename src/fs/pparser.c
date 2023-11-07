#include "pparser.h"
#include "string.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"


static int pathparser_path_valid_format(const char* filename)
{
    int len  = strnlen(filename, PERSONALOS_MAX_PATH);
    return(len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1],":/",2) == 0);
}

static int pathparser_get_drive_by_path(const char** path)
{
    if(!pathparser_path_valid_format(*path))
        return -EDADPATH;

    int drive_no = tonumericdigit(*path[0]); // get drive number

    //Add 3 bytes to skip drive number
    *path += 3;
    return drive_no;
}

static struct path_root* path_parser_create_root(int drive_number) // create root
{
    struct path_root* path_root = kzalloc(sizeof(struct path_root)); // allocate memory for path root
    path_root->drive_no = drive_number; // set drive number
    path_root->first = 0; // set first to 0
    return path_root; // return path root
}

static const char* pathparser_get_path_part(const char** path)
{
    char* result_path_part = kzalloc(PERSONALOS_MAX_PATH); // allocate memory for result path part
    int i = 0;
    while(**path != '/' && **path != 0x00) // while path is not equal to / and path is not equal to 0
    {
        result_path_part[i++] = **path; // set result path part to path
        (*path)++; // increment path
        i++
    }
    if(**path == '/') // if path is equal to /
    {   
        // Skip the forward slash to avoid  problems
        (*path)++; 

    }

    if(i == 0) // if i is equal to 0 we did not find a path part
    {
        kfree(result_path_part); // free result path part
        result_path_part = 0;
    }
    return result_path_part;
}

struct path_part* pathparser_parse_path_part(struct path_part* last , const char** path)
{
    const char* path_part_str = pathparser_get_path_part(path); // get path part
    if(!path_part_str) // if path part is 0
        return 0; // return 0

    struct path_part* path_part = kzalloc(sizeof(struct path_part)); // allocate memory for path part
    path_part->part = path_part_str; // set path part to path part str
    path_part->next = 0; // set next to 0

    if(last_part)
    {
        last_part->next = path_part; // set last part next to path part
    }
    return path_part; // return path part
}

void pathparser_free(struct path_root* root)
{
    struct path_part* part = root->first; // set current to first
    while(part)
    {
        struct path_part* next_part = part->next; // set next to next
        kfree((void*)part->part);
        kfree(part); // free part
        part = next_part; // set part to next
    }
    kfree(root); // free root
}

struct path_root* pathparser_parse(const char* path, const char* current_directory_path)
{
    int res = 0;
    const char* tmp_path = path; // set tmp path to path
    struct path_root path_root = 0; // set path root to 0

    if(strlen(path) > PERSONALOS_MAX_PATH) // if length of path is greater than personal os max path
        goto out; // goto out

    res = pathparser_get_drive_by_path(&tmp_path); // get drive by path
    if(res < 0) // if res is less than 0
        goto out; // goto out

    path_root = path_parser_create_root(res); // create root
    if(!path_root) // if path root is 0
    {
        goto out; // goto out
    }

    struct path_part* first_part = pathparser_parse_path_part(NULL, &tmp_path); // parse path part
    if(!first_part) // if first part is 0
    {
        goto out; // goto out
    }

    path_root->first = first_part; // set first to first part
    struct path_part* part = pathparser_parse_path_part(first_part, &tmp_path); // parse path part
    while(part)
    {
        part = pathparser_parse_path_part(part, &tmp_path); // parse path part
    }
out:
    return path_root;
}
 
