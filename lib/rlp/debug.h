#ifndef __RLP_DEBUG_H__
#define __RLP_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include <stdarg.h>
#include <stdbool.h>

evm_rlp_t rlp_init_immed(uint64_t type, ...);

bool rlp_test(evm_rlp_t r, evm_rlp_t r_exp);
bool rlp_immed(evm_rlp_t r, uint64_t type, ...);

#endif

evm_rlp_t rlp_init_byte_vec(byte_vec_t b);
evm_rlp_t rlp_init_list(evm_rlp_vec_t r);
evm_rlp_vec_t rlp_vec_init(uint64_t size);

void rlp_vec_free_rec(evm_rlp_vec_p r);
void rlp_free(evm_rlp_p r);

uint64_t rlp_decode_rec(
    evm_rlp_p out_r,
    uint64_t type,
    byte_p arr,
    uint64_t size
);

#endif