#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../utils/struct.h"
#include "../word/struct.h"

typedef uchar_vec_p evm_bytes_p;

void bytes_free(evm_bytes_p b);

evm_word_t bytes_get_word(evm_bytes_p b, uint64_t pos);
uchar_t bytes_get_byte(evm_bytes_p b, uint64_t pos);

#endif
