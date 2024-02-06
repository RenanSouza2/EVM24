#ifndef __BYTES_DEBUG_H__
#define __BYTES_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG

#include <stdbool.h>

uchar cton(uchar c);
void bytes_display(bytes_t b);
bytes_t bytes_init_immed(char str[]);
bool bytes_immed(bytes_t b, char str[]);

#endif

#endif
