#ifndef __BYTES_STRUCT_H__
#define __BYTES_STRUCT_H__

#include "header.h"
#include "../utils/types.h"

STRUCT(bytes)
{
    uchar *v;
    int size;
};

bytes_t bytes_init_zero();

#endif
