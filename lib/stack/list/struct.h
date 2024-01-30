#ifndef __STACK_LIST_STRUCT_H__
#define __STACK_LIST_STRUCT_H__

#include "header.h"
#include "../../word/struct.h"

#define S_MAX 1024

STRUCT(s_list)
{
    s_list_p sl;
    word_t w;
};

#endif
