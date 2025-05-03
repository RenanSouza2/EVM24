#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include <stdarg.h>
#include <stdbool.h>

evm_mem_t mem_init_variadic(uint64_t n, va_list *arg);
evm_mem_t mem_init_immed(uint64_t n, ...);

bool mem_immed(evm_mem_t m, uint64_t n, ...);

#endif

void mem_expand(evm_mem_p m, uint64_t pos);

#endif
