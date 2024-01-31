#ifndef __STACK_LIST_DEBUG_H__
#define __STACK_LIST_DEBUG_H__

#include "struct.h"

#ifdef DEBUG
#endif

stack_l_p stack_l_create(stack_l_p sl_next, word_t w);
stack_l_p stack_l_pop(stack_l_p sl);

#endif
