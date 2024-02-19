#ifndef __RLP_STRUCT_H__
#define __RLP_STRUCT_H__

#include "header.h"

#include "../utils/struct.h"

#define BYTES 0
#define STR 1

PLACEHOLDER(evm_rlp);

VEC(evm_rlp);

STRUCT(evm_rlp)
{
    uint64_t type;
    union
    {
        byte_vec_t b;
        evm_rlp_vec_t e;
    } vec;
};

#endif
