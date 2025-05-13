#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include "struct.h"

evm_stack_t stack_init();
void stack_free(evm_stack_p s);

uint64_t stack_push(evm_stack_p s, word_p w);
uint64_t stack_pop(word_p w, evm_stack_p s);

#endif
