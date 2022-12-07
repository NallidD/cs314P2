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
#include <unistd.h>
#include "bitmap.h"

#define FSSIZE 10485760 //2560 4K blocks
#define SID 0xBEEF
#define TOTAL_BLOCKS 2560
#define BLOCK_SIZE 4096 //4K bytes
#define TOTAL_DATA_BLOCKS 73400320 //total 512 byte data blocks in the fs
#define SUPER_OFFSET 4096
#define NODE_TABLE_OFFSET SUPER_OFFSET + BLOCK_SIZE
#define DATA_TABLE_OFFSET NODE_TABLE_OFFSET + BLOCK_SIZE
#define INODE_START 12288
#define DATA_START INODE_START * 63
#define FSFILE 0
#define FSDIR 1

#ifdef DEBUG
   #define D 1
#endif

/*

12288 + (inode_index * sizeof(inode))
63 4K blocks for inodes
2494 4K blocks for data

*/

extern unsigned char inode_bitmap[4096];
extern unsigned char data_bitmap[4096];

extern unsigned char* fs;
extern int data_index;
extern int inode_index;
extern int root_inode_index;

typedef struct SB {

    char fs_type[8];
    short block_total;
    int root_index; //data root index
    int data_index; //data start index
    char data_block_total;
    int inode_index;

} __attribute__((packed, aligned(4096))) super_block; //promised for 4096kb of space

typedef struct IN {

    char valid; //checks if inode is active
    char type;
    int size;
    int num_blocks;
    char ptr_to_block;

} __attribute__((packed, aligned(256))) inode;

typedef struct FBL {

    bitmap list[13]; //8 bits per list, 13 total indeces = 104 bits -> covers our 100 inodes

} __attribute__((packed, aligned(64))) free_list;

typedef struct DB {

    char data[512];

} __attribute((packed, aligned(512))) data_block;

typedef struct Directory {

    char * dir_name;
    char * files[64];
    int filesizes[100];
    int fileinodes[100];
    int num_files;
    struct Directory * next; //type cast to struct Directory use correctly.

} __attribute((packed, aligned(512))) dir;

extern dir * root;
extern inode * inodes;
extern inode * root_inode;
extern data_block * data;

void init_blocks(super_block * sb, inode * node, free_list * fl);
void print_blocks(super_block * sb, inode * node, free_list * fl);
void print_buffer();
void print_block(int bd); //bd = block descriptor
void rprint_block(int from, int to);
unsigned int get_next_data_block(unsigned int curindex);
inode * make_inode(int size, char type);
void write_inode(inode * iptr, int inum);
dir * make_dir();
void mapfs(int fd);
void unmapfs();
void formatfs();
void loadfs();
void lsfs();
void addfilefs(char* fname);
void removefilefs(char* fname);
void extractfilefs(char* fname);

#endif
