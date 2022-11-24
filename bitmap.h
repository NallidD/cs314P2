#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdlib.h>

typedef char * bitmap;

void set_bit(bitmap bits, int index);
void clear_bit(bitmap bits, int index);
char get_bit(bitmap bits, int index);
bitmap make_bitmap(int n);
void remove_bitmap(bitmap map);

#endif