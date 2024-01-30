#ifndef __STACK_LIST_STRUCT_H__
#define __STACK_LIST_STRUCT_H__

#include "header.h"
#include "../../word/struct.h"

#define S_MAX 1024

STRUCT(stack_list)
{
    word_t w;
    stack_list_p sl;
};

#endif
