#ifndef __BYTES_STRUCT_H__
#define __BYTES_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"
#include "../../word/struct.h"

typedef byte_vec_p evm_bytes_p;
typedef byte_vec_t evm_bytes_t;

byte_t bytes_get_byte(evm_bytes_p b, uint64_t pos);
word_t bytes_get_word(evm_bytes_p b, uint64_t pos);
evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t i, uint64_t size);

#endif
