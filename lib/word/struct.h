#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "header.h"

#define V_MAX 4

PLACEHOLDER(evm_bytes);

STRUCT(evm_word)
{
    uint64_t v[V_MAX];
};

evm_word_t word_init();
evm_word_t word_init_uint64(uint64_t num);
evm_word_t word_init_bytes(evm_bytes_p b);

void word_add_uint64(evm_word_p w, int i, uint64_t v);

evm_word_t word_add(evm_word_p w1, evm_word_p w2);

#endif
