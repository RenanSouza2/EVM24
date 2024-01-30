#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "header.h"

#define V_MAX 4
#define U64_MAX (~(u64)0)

typedef __uint64_t u64;

STRUCT(word)
{
    u64 v[V_MAX];
};

#endif
