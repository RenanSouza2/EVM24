#ifndef __VEC_STRUCT_H__
#define __VEC_STRUCT_H__

#include <stdint.h>

#include "header.h"
#include "../utils/struct.h"

STRUCT(uint64_vec)
{
    uint64_t size;
    uint64_p v;
};

uint64_vec_t uint64_vec_init(uint64_t size);

#endif
