#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"

#define V_MAX 4

STRUCT(evm_word)
{
    uint64_t v[V_MAX];
};

evm_word_t word_init();
evm_word_t word_init_uint64(uint64_t num);
evm_word_t word_init_bytes(byte_vec_p b);

bool word_is_uint64(evm_word_p w);
bool word_eq_bool(evm_word_p w1, evm_word_p w2);
void word_add_uint64(evm_word_p w, uint64_t i, uint64_t v);
byte_t word_get_byte(evm_word_p w, uint64_t i);
void word_set_byte(evm_word_p w, uint64_t i, byte_t u);

evm_word_t word_add(evm_word_p w1, evm_word_p w2);

#endif
