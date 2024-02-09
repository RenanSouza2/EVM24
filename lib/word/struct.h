#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include <stdint.h>

#include "header.h"
#include "../utils/struct.h"

#define V_MAX 4

PLACEHOLDER(evm_bytes);

STRUCT(evm_word)
{
    uint64_t v[V_MAX];
};

evm_word_t word_from_zero();
evm_word_t word_from_bytes(evm_bytes_p b);

#endif
