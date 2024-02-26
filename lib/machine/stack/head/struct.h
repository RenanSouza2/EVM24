#ifndef __STACK_HEAD_STRUCT_H__
#define __STACK_HEAD_STRUCT_H__

#include "header.h"

#include "../list/struct.h"

STRUCT(evm_stack)
{
    evm_stack_l_p sl;
    uint64_t count;
};

evm_stack_t stack_init();
void stack_free(evm_stack_p s);

uint64_t stack_push(evm_stack_p s, evm_word_p w);
uint64_t stack_pop(evm_word_p w, evm_stack_p s);

#endif
