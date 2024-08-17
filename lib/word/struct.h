#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "header.h"

#include "../utils/struct.h"

#define V_MAX 4

STRUCT(word)
{
    uint64_t arr[V_MAX];
};

word_t word_init_zero();
word_t word_init_uint64(uint64_t num);
word_t word_init_bytes(byte_vec_p b);

bool word_is_uint64(word_p w);
bool word_eq_bool(word_p w1, word_p w2);
void word_add_uint64(word_p w, uint64_t i, uint64_t v);
byte_t word_get_byte(word_p w, uint64_t i);
void word_set_byte(word_p w, uint64_t i, byte_t u);

word_t word_add(word_p w1, word_p w2);

#endif
