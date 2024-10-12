#ifndef __RLP_STRUCT_H__
#define __RLP_STRUCT_H__

#include "header.h"

#include "../utils/struct.h"

#define BYTES 0
#define LIST 1

PLACEHOLDER(evm_rlp);

VEC_DEFINE(evm_rlp);

STRUCT(evm_rlp)
{
    uint64_t type;
    union
    {
        byte_vec_t b;
        evm_rlp_vec_t r;
    } vec;
};

byte_vec_t rlp_encode(evm_rlp_p r);
uint64_t rlp_decode(evm_rlp_p out_r, byte_vec_p b);

#endif
