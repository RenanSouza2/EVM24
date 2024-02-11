#ifndef __BYTES_STRUCT_H__
#define __BYTES_STRUCT_H__

#include "header.h"
#include "../utils/struct.h"

STRUCT(evm_bytes)
{
    uchar *v;
    int size;
};

evm_bytes_t bytes_init_zero();
evm_bytes_t bytes_get_bytes(evm_bytes_p b, int i, int size);

#endif
