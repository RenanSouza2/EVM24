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
void bytes_free(bytes_t b);

void bytes_expand(bytes_p b, int size);
uchar bytes_get_byte(bytes_p b, int i);
void bytes_set_byte(bytes_p b, int i, uchar u);
word_t bytes_get_word(bytes_p b, int i);
void bytes_set_word(bytes_p b, int i, word_p w);
bytes_t bytes_get_bytes(bytes_p b, int i, int size);

#endif
