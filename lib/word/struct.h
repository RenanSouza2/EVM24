#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include <stdint.h>

#include "header.h"
#include "../utils/struct.h"

#define V_MAX 4

PLACEHOLDER(bytes);

STRUCT(word)
{
    uint64_t v[V_MAX];
};

word_t word_from_zero();
word_t word_from_bytes(bytes_p b);

#endif
