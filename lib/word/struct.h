#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "header.h"
#include "../utils/types.h"

#define V_MAX 4

PLACEHOLDER(bytes);

STRUCT(word)
{
    u64 v[V_MAX];
};

word_t word_from_zero();
word_t word_from_bytes(bytes_p b);

#endif
