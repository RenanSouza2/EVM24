#ifndef __STACK_HEAD_STRUCT_H__
#define __STACK_HEAD_STRUCT_H__

#include "header.h"

PLACEHOLDER(evm_stack_l);

STRUCT(evm_stack)
{
    evm_stack_l_p sl;
    int count;
};

evm_stack_t stack_init();

#endif
