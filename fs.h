#ifndef __FS_H__
#define __FS_H__
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "bitmap.h"

#define FSSIZE 10485760

extern unsigned char inode_bitmap[4096];
extern unsigned char data_bitmap[4096];

extern unsigned char* fs;

typedef struct SB {

    char fs_type[8];
    short block_total;
    short root_index;
    short data_index;
    char data_block_total;
    char num_blocks;

} __attribute__((packed, aligned(4096))) super_block; //promised for 4096kb of space

typedef struct IN {

    char valid; //checks if inode is active
    char type;
    int size;
    int num_blocks;
    char direct[100];

} __attribute__((packed, aligned(4096))) inode;

typedef struct FBL {

    bitmap list[2];

} __attribute__((packed, aligned(4096))) free_list;

typedef struct DB {

    char data[512];

} __attribute((packed, aligned(512))) data_block;

typedef struct Directory {

    char * files[64];
    int filesizes[100];
    int fileinodes[100];
    int num_files;

} dir;

void init_blocks(super_block * sb, inode * node, free_list * fl);
void print_blocks(super_block * sb, inode * node, free_list * fl);
void print_buffer();
void write_to_buffer(super_block * sb, inode * node, free_list * fl);
unsigned int get_next_data_block();
void mapfs(int fd);
void unmapfs();
void formatfs();
void loadfs();
void lsfs();
void addfilefs(char* fname);
void removefilefs(char* fname);
void extractfilefs(char* fname);

#endif
