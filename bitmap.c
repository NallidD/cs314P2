#include "bitmap.h"

void set_bit(bitmap bits, int index){

    bits[index / 8] |= 1 << (index & 7);

}

void clear_bit(bitmap bits, int index) {

    bits[index / 8] &= ~(1 << (index & 7));

}

char get_bit(bitmap bits, int index) {

    return bits[index / 8] & (1 << (index & 7)) ? 1 : 0;

}

bitmap make_bitmap(int n) {

    return (bitmap)malloc((n + 7) / 8);

}

void remove_bitmap(bitmap map) {

    free(map);

}