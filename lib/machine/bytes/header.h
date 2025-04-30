#ifndef __BYTES_H__
#define __BYTES_H__

#include "struct.h"

byte_t bytes_get_byte(evm_bytes_p b, uint64_t pos);
word_t bytes_get_word(evm_bytes_p b, uint64_t pos);
evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t i, uint64_t size);

#endif
