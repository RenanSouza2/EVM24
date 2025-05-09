#ifndef __STACK_LIST_DEBUG_H__
#define __STACK_LIST_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include <stdbool.h>
#include <stdarg.h>

void stack_l_display(evm_stack_l_p sl);

evm_stack_l_p stack_l_init_variadic(uint64_t n, va_list *args);

bool stack_l_test(evm_stack_l_p sl_1, evm_stack_l_p sl_2);
bool stack_l_immed(evm_stack_l_p sl, uint64_t n, ...);

#endif

#endif
