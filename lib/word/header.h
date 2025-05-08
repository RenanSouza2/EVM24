#ifndef __WORD_H__
#define __WORD_H__

#include "struct.h"

#include <stdbool.h>

word_t word_init_bytes(byte_vec_p b);

bool word_is_uint64(word_p w);
bool word_eq(word_p w1, word_p w2);
void word_add_uint64(word_p w, uint64_t i, uint64_t v);
byte_t word_get_byte(word_p w, uint64_t i);
void word_set_byte(word_p w, uint64_t i, byte_t u);
byte_vec_t byte_vec_init_word(word_p w);

word_t word_add(word_p w1, word_p w2);

#endif
