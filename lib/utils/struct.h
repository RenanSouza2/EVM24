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

#define VEC(TYPE)       \
    STRUCT(TYPE##_vec)  \
    {                   \
        uint64_t size;  \
        TYPE##_p v;     \
    }

VEC(uint64);
VEC(byte);

#define DECL_VEC_INIT(TYPE) TYPE##_vec_t TYPE##_vec_init(uint64_t size)
#define DECL_VEC_FREE(TYPE) void TYPE##_vec_free(TYPE##_vec_p vec)

byte_vec_t byte_vec_init_zero();
DECL_VEC_INIT(byte);
DECL_VEC_INIT(uint64);

DECL_VEC_FREE(byte);
DECL_VEC_FREE(uint64);

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v);

#endif
