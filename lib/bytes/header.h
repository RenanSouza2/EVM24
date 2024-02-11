#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../utils/struct.h"

PLACEHOLDER(evm_bytes);

#include "../word/struct.h"

void bytes_free(evm_bytes_p b);

void bytes_expand(evm_bytes_p b, int size);

evm_word_t bytes_get_word(evm_bytes_p b, int i);
uchar bytes_get_byte(evm_bytes_p b, int i);

void bytes_set_word(evm_bytes_p b, int i, evm_word_p w);
void bytes_set_byte(evm_bytes_p b, int i, uchar u);

#endif
