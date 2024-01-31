#ifndef __STACK_LIST_DEBUG_H__
#define __STACK_LIST_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>
#include <stdarg.h>

void stack_l_display(stack_l_p sl);
bool stack_l(stack_l_p sl, int n, ...);
bool stack_l_var(stack_l_p sl, int n, va_list args);

#endif

#endif
