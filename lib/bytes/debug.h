#ifndef __BYTES_DEBUG_H__
#define __BYTES_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG

uchar cton(uchar c);
void bytes_display(bytes_t b);
bytes_t bytes_init_immed(char str[]);

#endif

bytes_t bytes_init(uchar b[], int size);

bytes_t bytes_get_bytes(bytes_p b, int i, int size);

#endif
