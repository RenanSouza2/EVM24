#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

#define U64_MAX UINT64_MAX
#define U64_FF 0xff00000000000000

#define IGN UINT64_MAX

#undef  assert
#define assert(COND)                                                        \
        if(!(COND))                                                         \
        {                                                                   \
            printf("\n\n");                                                 \
            __assert_fail(#COND, __FILE__, __LINE__, __ASSERT_FUNCTION);    \
        }

#define assert_int(RES, EXP) assert(int_test(RES, EXP))
#define assert_64(RES, EXP) assert(uint64_test(RES, EXP));

bool uchar_test(uchar_t u1, uchar_t u2);
bool int_test(int i1, int i2);
bool uint64_test(uint64_t i1, uint64_t i2);
bool uint64_vec_test_immed(uint64_vec_t vec, uint64_t n, ...);

#endif

#endif
