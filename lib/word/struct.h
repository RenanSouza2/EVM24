#ifndef __WORD_STRUCT_H__
#define __WORD_STRUCT_H__

#include "../utils/struct.h"

#define V_MAX 4

STRUCT(word)
{
    uint64_t arr[V_MAX];
};

#define W4(V3, V2, V1, V0) ((word_t){{V0, V1, V2, V3}})
#define W1(V0) ((word_t){{V0, 0, 0, 0}})

#endif
