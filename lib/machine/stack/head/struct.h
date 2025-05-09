#ifndef __STACK_HEAD_STRUCT_H__
#define __STACK_HEAD_STRUCT_H__

#include "../list/struct.h"

STRUCT(evm_stack)
{
    evm_stack_l_p sl;
    uint64_t count;
};

#endif
