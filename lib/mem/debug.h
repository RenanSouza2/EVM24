#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG

#include <stdarg.h>

evm_mem_t mem_init_immed(int n, ...);
evm_mem_t mem_init_immed_variadic(int n, va_list *arg);

bool mem_test_immed(evm_mem_t m, int n, ...);
bool mem_test_variadic(evm_mem_t m, int n, va_list *args);

#endif

void mem_expand(evm_mem_p m, int i);

#endif
