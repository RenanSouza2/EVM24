#ifndef __STACK_HEAD_DEBUG_H__
#define __STACK_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include "../../word/struct.h"

void stack_display(stack_d_t s);
stack_d_t stack_init_immed_variadic(int n, va_list *args);
stack_d_t stack_init_immed(int n, ...);
bool stack_immed(stack_d_t s, int n, ...);
bool stack_immed_variadic(stack_d_t s, int n, va_list args);
bool stack_d_push_immed(stack_d_p s, word_t w);

#endif

#endif
