#ifndef __STACK_LIST_DEBUG_H__
#define __STACK_LIST_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>
#include <stdarg.h>

void stack_l_display(evm_stack_l_p sl);

evm_stack_l_p stack_l_init_immed_variadic(int n, va_list *args);

bool stack_l_test_immed(evm_stack_l_p sl, int n, ...);
bool stack_l_test_variadic(evm_stack_l_p sl, int n, va_list *args);

#endif

#endif
