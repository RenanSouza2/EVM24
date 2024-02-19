#ifndef __RLP_STRUCT_H__
#define __RLP_STRUCT_H__

#include "header.h"

#include "../utils/struct.h"

#define BYTES 0
#define STR 1

STRUCT(evm_rlp)
{
    uint64_t type;
    vec_t vec;
};

#endif
