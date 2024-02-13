#ifndef __UTILS_STRUCT_H__
#define __UTILS_STRUCT_H__

#include <stdint.h>

#include "header.h"

typedef unsigned char uchar_t;
typedef uchar_t * uchar_p;
typedef __uint128_t uint128_t;
typedef uint64_t * uint64_p;

uint64_t uint64_add(uint64_t u1, uint64_t u2);
uint64_t uint128_to_uint64(uint128_t res);

#endif
