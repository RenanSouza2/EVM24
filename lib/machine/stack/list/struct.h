#ifndef __STACK_LIST_STRUCT_H__
#define __STACK_LIST_STRUCT_H__

#include "header.h"

#include "../../word/struct.h"

#define S_MAX 1024

STRUCT(evm_stack_l)
{
    evm_stack_l_p sl;
    evm_word_t w;
};

#endif
