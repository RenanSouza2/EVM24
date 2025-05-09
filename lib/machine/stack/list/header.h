#ifndef __STACK_LIST_H__
#define __STACK_LIST_H__

#include "struct.h"

evm_stack_l_p stack_l_create(evm_stack_l_p sl_next, word_p w);
evm_stack_l_p stack_l_pop(word_p w, evm_stack_l_p sl);
void stack_l_free(evm_stack_l_p sl);

#endif
