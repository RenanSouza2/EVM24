#ifndef __STACK_HEAD_DEBUG_H__
#define __STACK_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include "../../../word/struct.h"

void stack_display(evm_stack_t s);

evm_stack_t stack_init_immed_variadic(uint64_t n, va_list *args);

bool stack_test_immed(evm_stack_t s, uint64_t n, ...);
bool stack_test_variadic(evm_stack_t s, uint64_t n, va_list *args);

uint64_t stack_push_immed(evm_stack_p s, evm_word_t w);

#endif

#endif
