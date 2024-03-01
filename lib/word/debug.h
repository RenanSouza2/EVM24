#ifndef __WORD_DEBUG_H__
#define __WORD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define WORD(V3, V2, V1, V0) ((word_t){{V0, V1, V2, V3}})
#define W1(V0) ((word_t){{V0, 0, 0, 0}})

void word_display(word_t w);
void word_display_immed(word_t w);

bool word_test(word_t w1, word_t w2);

#endif

bool word_eq_bool(word_p w1, word_p w2);

#endif
