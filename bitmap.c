#include "bitmap.h"

void set_bit(bitmap bits, int index) {

    bits[index / 8] |= 1 << (index & 7);

}

void unset_bit(bitmap bits, int index) {

    bits[index / 8] &= ~(1 << (index & 7));

}

char grab_bit(bitmap bits, int index) {

    return bits[index / 8] & (1 << (index & 7)) ? 1 : 0;

}

bitmap create(int size) {

    return malloc((size + 7) / 8);

}