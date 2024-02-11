#ifndef __STACK_HEAD_STRUCT_H__
#define __STACK_HEAD_STRUCT_H__

#include "header.h"
#include "../list/header.h"

STRUCT(evm_stack)
{
    evm_stack_l_p sl;
    int count;
};

evm_stack_t stack_init();

#endif
