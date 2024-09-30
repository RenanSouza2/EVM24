#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <assert.h>

#undef assert

#ifdef __linux__

#define assert(COND)                                                 \
    if (!(COND))                                                     \
    {                                                                \
        printf("\n\n");                                              \
        __assert_fail(#COND, __FILE__, __LINE__, __ASSERT_FUNCTION); \
    }

#elif defined __APPLE__

#define assert(COND)                                                 \
    if (!(COND))                                                     \
    {                                                                \
        printf("\n\n");                                              \
        __assert_rtn(__func__, __ASSERT_FILE_NAME, __LINE__, #COND); \
    }

#endif

#endif
