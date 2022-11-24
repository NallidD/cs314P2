#ifndef __FS_H__
#define __FS_H__
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "bitmap.h"

#define FSSIZE 10485760

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

    char valid;
    char size;
    char direct[100];

} __attribute__((packed, aligned(4096))) inode;

typedef struct FBL {

    bitmap list[2];

} __attribute__((packed, aligned(4096))) free_list;

void init_blocks(super_block * sb, inode * node, free_list * fl);
void print_blocks(super_block * sb, inode * node, free_list * fl);
void print_buffer();
void mapfs(int fd);
void unmapfs();
void formatfs();
void loadfs();
void lsfs();
void addfilefs(char* fname);
void removefilefs(char* fname);
void extractfilefs(char* fname);

#endif
