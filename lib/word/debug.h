#ifndef __WORD_DEBUG_H__
#define __WORD_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define WORD(V3, V2, V1, V0) ((evm_word_t){{V0, V1, V2, V3}})
#define W1(V0) ((evm_word_t){{V0, 0, 0, 0}})

void word_display(evm_word_t w);
void word_display_immed(evm_word_t w);

bool word_test(evm_word_t w1, evm_word_t w2);

#endif

bool word_eq_bool(evm_word_p w1, evm_word_p w2);
void word_add_immed(evm_word_p w, int i, uint64_t v);

evm_word_t word_add(evm_word_p w1, evm_word_p w2);

#endif
