#ifndef __RLP_STRUCT_H__
#define __RLP_STRUCT_H__

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

#endif
