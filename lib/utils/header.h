#ifndef __UTILS_H__
#define __UTILS_H__

#include "../../utils/struct.h"

#include <assert.h>

#undef  assert
#define assert(COND)        \
    if(!(COND))             \
    {                       \
        printf("\n\n");     \
        __assert_fail(#COND, __FILE__, __LINE__, __ASSERT_FUNCTION);    \
    }

#endif
