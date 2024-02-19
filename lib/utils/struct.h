#ifndef __UTILS_STRUCT_H__
#define __UTILS_STRUCT_H__

#include <stdint.h>
#include <stdbool.h>

#include "header.h"

#ifdef __linux__

#define U64P  "%lu"
#define U64PX "%016lx"

#elif defined __APPLE__

#define U64P  "%llu"
#define U64PX "%016llx"

#endif

typedef unsigned char byte_t;
typedef byte_t * byte_p;
typedef uint64_t * uint64_p;
typedef __uint128_t uint128_t;

uint64_t uint64_add(uint64_t u1, uint64_t u2);
uint64_t uint128_to_uint64(uint128_t res);

STRUCT(vec)         \
{                   \
    uint64_t size;  \
    handler_p v;    \
};

#define VEC(TYPE)       \
    STRUCT(TYPE##_vec)  \
    {                   \
        uint64_t size;  \
        TYPE##_p v;     \
    }

VEC(uint64);
VEC(byte);

uint64_vec_t uint64_vec_init(uint64_t size);

void vec_free(handler_p vec);

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v);

#endif
