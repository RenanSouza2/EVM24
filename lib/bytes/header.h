#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../utils/struct.h"

PLACEHOLDER(evm_bytes);

#include "../word/struct.h"

void bytes_free(evm_bytes_p b);

void bytes_expand(evm_bytes_p b, uint64_t size);

evm_word_t bytes_get_word(evm_bytes_p b, uint64_t pos);
uchar_t bytes_get_byte(evm_bytes_p b, uint64_t pos);

void bytes_set_word(evm_bytes_p b, uint64_t pos, evm_word_p w);
void bytes_set_byte(evm_bytes_p b, uint64_t pos, uchar_t u);
void bytes_set_bytes(evm_bytes_p b, uint64_t pos, evm_bytes_p b_data);

#endif

// TODO remove from bytes expansion 