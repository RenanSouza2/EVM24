#ifndef __STACK_LIST_STRUCT_H__
#define __STACK_LIST_STRUCT_H__

#include "header.h"

#include "../../../word/struct.h"

#define S_MAX 1024

STRUCT(evm_stack_l)
{
    evm_stack_l_p sl;
    word_t w;
};

evm_stack_l_p stack_l_create(evm_stack_l_p sl_next, word_p w);
evm_stack_l_p stack_l_pop(word_p w, evm_stack_l_p sl);
void stack_l_free(evm_stack_l_p sl);

#endif
