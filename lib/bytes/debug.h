#ifndef __BYTES_DEBUG_H__
#define __BYTES_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

uchar cton(uchar c);
bytes_t bytes_init_immed(char str[]);

#endif

bytes_t bytes_init(uchar b[], int size);
void bytes_free(bytes_t b);
uchar bytes_access(bytes_t b, int i);

#endif
