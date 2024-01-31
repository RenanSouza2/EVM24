#ifndef __STACK_HEAD_DEBUG_H__
#define __STACK_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

void stack_display(stack_t s);
bool stack(stack_t s, int n, ...);

#endif

#endif
