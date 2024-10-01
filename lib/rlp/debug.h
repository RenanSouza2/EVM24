#ifndef __RLP_DEBUG_H__
#define __RLP_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

evm_rlp_t rlp_init_immed(uint64_t type, ...);

byte_vec_t rlp_encode_immed(uint64_t type, ...);
uint64_t rlp_decode_immed(evm_rlp_p r, char str[]);

bool rlp_test_immed(evm_rlp_t r, uint64_t type, ...);
bool rlp_test(evm_rlp_t r, evm_rlp_t r_exp);
bool rlp_vec_test(evm_rlp_vec_t r, evm_rlp_vec_t r_exp);

#endif

evm_rlp_t rlp_init_byte_vec(byte_vec_p b);
evm_rlp_t rlp_init_list(evm_rlp_vec_p r);
evm_rlp_vec_t rlp_vec_init(uint64_t size);

void rlp_vec_free_rec(evm_rlp_vec_p r);
void rlp_free(evm_rlp_p r);

uint64_t rlp_decode_rec(evm_rlp_p out_r, uint64_t type, byte_p b, uint64_t size);
uint64_t rlp_decode(evm_rlp_p r, byte_vec_p b);

#endif