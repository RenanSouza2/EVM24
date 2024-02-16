#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

#define U128(U641, U642) ((((uint128_t)(U641)) << 64) | (uint128_t)(U642))

#define U64_MAX     UINT64_MAX
#define U64_FF      0xff00000000000000
#define U128_1      U128(1, 0)
#define U128_MAX    U128(UINT64_MAX, UINT64_MAX)

#define IGN UINT64_MAX

#define assert_int(RES, EXP) assert(int_test(RES, EXP))
#define assert_64(RES, EXP) assert(uint64_test(RES, EXP));

uint64_vec_t uint64_vec_init_immed(uint64_t n, ...);

bool uchar_test(uchar_t u1, uchar_t u2);
bool int_test(int i1, int i2);
bool uint64_test(uint64_t i1, uint64_t i2);
bool uint64_vec_test_immed(uint64_vec_t vec, uint64_t n, ...);

#endif

#endif
