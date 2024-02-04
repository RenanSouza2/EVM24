#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../utils/struct.h"
#include "../word/struct.h"

PLACEHOLDER(bytes);

void bytes_free(bytes_p b);

void bytes_expand(bytes_p b, int size);
word_t bytes_get_word(bytes_p b, int i);
void bytes_set_word(bytes_p b, int i, word_p w);
void bytes_set_byte(bytes_p b, int i, uchar u);

#endif
