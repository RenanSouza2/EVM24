#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include <stdint.h>

#include "header.h"
#include "../utils/struct.h"

#define V_MAX 4

PLACEHOLDER(evm_bytes);

STRUCT(evm_word)
{
    uint64_t v[V_MAX];
};

evm_word_t word_init();
evm_word_t word_init_uint_64(uint64_t i);
evm_word_t word_init_bytes(evm_bytes_p b);

evm_word_t word_add(evm_word_p w1, evm_word_p w2);

#endif
