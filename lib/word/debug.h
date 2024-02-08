#ifndef __WORD_DEBUG_H__
#define __WORD_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define WORD(V3, V2, V1, V0) ((word_t){{V0, V1, V2, V3}})
#define W1(V0) ((word_t){{V0, 0, 0, 0}})

void word_display(word_t w);
void word_display_immed(word_t w);

bool word(word_t w1, word_t w2);
bool word_immed(word_t w, uint64_t v3, uint64_t v2, uint64_t v1, uint64_t v0);

#endif

bool word_eq_bool(word_p w1, word_p w2);
void word_add_immed(word_p w, int i, uint64_t v);

word_t word_add(word_p w1, word_p w2);

#endif
