#ifndef __MEM_STRUCT_H__
#define __MEM_STRUCT_H__

#include "header.h"
#include "../bytes/struct.h"

typedef evm_bytes_t evm_mem_t;

evm_mem_t mem_init();
void mem_free(evm_mem_t m);

#endif
