#ifndef __STACK_LIST_H__
#define __STACK_LIST_H__

#include "../../../utils/struct.h"
#include "../../word/header.h"

PLACEHOLDER(stack_l);

stack_l_p stack_l_create(stack_l_p sl_next, word_p w);
stack_l_p stack_l_pop(stack_l_p sl);
void stack_l_free(stack_l_p sl);

#endif
