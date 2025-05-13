#ifndef __STACK_LIST_STRUCT_H__
#define __STACK_LIST_STRUCT_H__

#include "../../../word/struct.h"

#define S_MAX 1024

STRUCT(evm_stack_l)
{
    evm_stack_l_p sl;
    word_t w;
};

#endif
