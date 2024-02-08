#ifndef __STACK_HEAD_STRUCT_H__
#define __STACK_HEAD_STRUCT_H__

#include "header.h"
#include "../list/header.h"

STRUCT(stack_d)
{
    stack_l_p sl;
    int count;
};

stack_d_t stack_init();

#endif
