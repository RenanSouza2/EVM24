#ifndef __STACK_HEAD_DEBUG_H__
#define __STACK_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include "../../word/struct.h"

void stack_display(stack_t s);
bool stack(stack_t s, int n, ...);
bool stack_push_immed(stack_p s, word_t w);

#endif

#endif
