#ifndef __UTILS_STRUCT_H__
#define __UTILS_STRUCT_H__

#include "../../mods/macros/struct.h"
#include "../../mods/macros/U64.h"

typedef unsigned char byte_t;
typedef byte_t * byte_p;
typedef uint64_t * uint64_p;
typedef __uint128_t uint128_t;

STRUCT(vec)
{
    uint64_t size;
    handler_p arr;
};

#define VEC(POINTER) ((vec_p)(POINTER))

#define VEC_DEFINE(TYPE)    \
    STRUCT(TYPE##_vec)      \
    {                       \
        uint64_t size;      \
        TYPE##_p arr;       \
    }

VEC_DEFINE(uint64);
VEC_DEFINE(byte);

#endif
