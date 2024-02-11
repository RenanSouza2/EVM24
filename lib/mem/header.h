#ifndef __MEM_H__
#define __MEM_H__

#include "../../utils/struct.h"
#include "../bytes/struct.h"

typedef evm_bytes_p evm_mem_p;

int mem_dry_run(evm_mem_p m, uint64_t pos);

evm_word_t mem_get_word(evm_mem_p m, uint64_t pos);
evm_bytes_t mem_get_bytes(evm_mem_p m, uint64_t pos, uint64_t size);

void mem_set_word(evm_mem_p m, uint64_t pos, evm_word_p w);
void mem_set_byte(evm_mem_p m, uint64_t pos, uchar u);

#endif
