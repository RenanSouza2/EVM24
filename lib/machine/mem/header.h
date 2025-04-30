#ifndef __MEM_H__
#define __MEM_H__

#include "struct.h"

uint64_t mem_dry_run(evm_mem_p m, word_t w_pos, uint64_t size);
void mem_expand(evm_mem_p m, uint64_t pos);

word_t mem_get_word(evm_mem_p m, uint64_t pos);
byte_vec_t mem_get_bytes(evm_mem_p m, uint64_t pos, uint64_t size);

void mem_set_byte(evm_mem_p m, uint64_t pos, byte_t u);
void mem_set_word(evm_mem_p m, uint64_t pos, word_p w);
void mem_set_bytes(evm_mem_p m, uint64_t pos, byte_vec_p b);

#endif
