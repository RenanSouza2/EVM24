#ifndef __UTILS_STRUCT_H__
#define __UTILS_STRUCT_H__

#include <stdint.h>

#include "header.h"

typedef unsigned char uchar_t;
typedef uchar_t * uchar_p;
typedef __uint128_t uint128_t;
typedef uint64_t * uint64_p;

#define VEC(TYPE)       \
    STRUCT(TYPE##_vec)  \
    {                   \
        uint64_t size;  \
        TYPE##_p v;     \
    }

VEC(uint64);
VEC(uchar);

uint64_t uint64_add(uint64_t u1, uint64_t u2);
uint64_t uint128_to_uint64(uint128_t res);

uint64_vec_t uint64_vec_init(uint64_t size);
void uint64_vec_free(uint64_vec_t vec);

#endif
