#ifndef __RLP_H__
#define __RLP_H__

#include "struct.h"

byte_vec_t rlp_encode(evm_rlp_p r);
uint64_t rlp_decode(evm_rlp_p out_r, byte_vec_p b);

evm_rlp_vec_t rlp_vec_init(uint64_t size);
evm_rlp_t rlp_init_byte_vec(byte_vec_t b);
evm_rlp_t rlp_init_list(evm_rlp_vec_t r);

#endif
