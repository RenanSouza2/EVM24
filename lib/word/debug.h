#ifndef __WORD_DEBUG_H__
#define __WORD_DEBUG_H__

#include "header.h"

#ifdef DEBUG

void word_display(word_t w);

bool word_test(word_t w1, word_t w2);

#endif

bool word_eq(word_p w1, word_p w2);

#endif
