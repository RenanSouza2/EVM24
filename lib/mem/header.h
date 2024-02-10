#ifndef __MEM_H__
#define __MEM_H__

#include "../../utils/struct.h"
#include "../bytes/header.h"

typedef evm_bytes_p evm_mem_p;

int mem_dry_run(evm_mem_p m, int i);

evm_word_t mem_get_word(evm_mem_p m, int i);
void mem_set_word(evm_mem_p m, int i, evm_word_p w);

#endif
