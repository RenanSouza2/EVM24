#ifndef __WORD_DEBUG_H__
#define __WORD_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define WORD(V3, V2, V1, V0) ((word_t){{V0, V1, V2, V3}})

bool word_immed(word_t w, u64 v3, u64 v2, u64 v1, u64 v0);

#endif

bool word_eq_bool(word_p w1, word_p w2);
void word_add_immed(word_p w, int i, u64 v);
word_t word_add(word_t w1, word_t w2);

#endif
