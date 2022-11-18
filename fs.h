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

#define FSSIZE 10000000
#define DIRECTSIZE 12
#define NULLBLOCK 0
#define VALIDBLOCK 1
#define SBBEGIN 0
#define SBEND 16
#define FLBEGIN 17
#define FLEND 19
#define INBEGIN 20
#define INEND 122

extern unsigned char* fs;
#pragma pack(push)
typedef struct SB {

    short block;

} super_block;
#pragma pack(pop)
#pragma pack(push)
typedef struct IN {

    char valid;
    char size;
    char direct[100];

} inode;
#pragma pack(pop)
#pragma pack(push)
typedef struct FBL {

    bitmap list[2];

} free_list;
#pragma pack(pop)

void mapfs(int fd);
void unmapfs();
void formatfs();
void loadfs();
void lsfs();
void addfilefs(char* fname);
void removefilefs(char* fname);
void extractfilefs(char* fname);

#endif
