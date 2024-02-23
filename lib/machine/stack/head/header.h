#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include "../../../../utils/struct.h"

PLACEHOLDER(evm_word);
PLACEHOLDER(evm_stack);

void stack_free(evm_stack_p s);
int stack_push(evm_stack_p s, evm_word_p w);
int stack_pop(evm_word_p w, evm_stack_p s);

#endif
