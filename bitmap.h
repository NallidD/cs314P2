#include <stdlib.h>

typedef char * bitmap;

void set_bit(bitmap bits, int index);
void unset_bit(bitmap bits, int index);
char grab_bit(bitmap bits, int index);
bitmap create(int size);