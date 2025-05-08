#ifndef __STACK_HEAD_DEBUG_H__
#define __STACK_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

#include "../../../word/struct.h"


evm_stack_t stack_init_variadic(uint64_t n, va_list *args);
evm_stack_t stack_init_immed(uint64_t n, ...);

void stack_display(evm_stack_t s);

bool stack_test(evm_stack_t s_1, evm_stack_t s_2);
bool stack_immed(evm_stack_t s, uint64_t n, ...);

uint64_t stack_push_immed(evm_stack_p s, word_t w);

#endif

#endif
