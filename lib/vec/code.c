#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

uint64_vec_t uint64_vec_init(uint64_t size)
{
    uint64_p v = calloc(size, sizeof(uint64_t));
    assert(v);

    return (uint64_vec_t){size, v};
}

void uint64_vec_free(uint64_vec_t vec)
{
    if(vec.v) free(vec.v);
}