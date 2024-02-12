#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

#define U64_MAX UINT64_MAX
#define U64_FF 0xff00000000000000

#define IGN UINT64_MAX

#define _assert(COND)                \
    {                               \
        bool res = COND;            \
        if(!res) printf("\n\n");    \
        assert(res);                \
    }

#define assert_int(RES, EXP) _assert(int_test(RES, EXP))
#define assert_64(RES, EXP) _assert(uint64_test(RES, EXP));

void uint64_display(uint64_t u);

bool uchar_test(uchar_t u1, uchar_t u2);
bool int_test(int i1, int i2);
bool uint64_test(uint64_t i1, uint64_t i2);

#endif

#endif
