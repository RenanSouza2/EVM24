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

#define TRY(FN)                 \
    {                           \
        uint64_t res = (FN);    \
        if(res)

#define CATCH }

#define ERR_CONCAT(RES, CODE) ((res) << 4) | (CODE)

#define ERR(FN, CODE)               \
    TRY(FN)                         \
        return (CODE)               \
            ? ERR_CONCAT(res, CODE) \
            : res;                  \
    CATCH

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

uint64_t uint64_add(uint64_t u1, uint64_t u2);
uint64_t uint64_init_byte_arr(byte_p b, uint64_t size);
uint64_t uint128_to_uint64(uint128_t res);

#define DECL_VEC_INIT(TYPE) TYPE##_vec_t TYPE##_vec_init(uint64_t size)

byte_vec_t byte_vec_init_zero();
byte_vec_t byte_vec_init_uint64(uint64_t num);
byte_vec_t byte_vec_init(uint64_t size);
byte_vec_t byte_vec_init_byte_arr(byte_p b, uint64_t size);
byte_vec_t byte_vec_concat(byte_vec_p b1, byte_vec_p b2);

uint64_vec_t uint64_vec_init_zero();
uint64_vec_t uint64_vec_init(uint64_t size);
bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v);

void vec_free(handler_p v);

#endif
