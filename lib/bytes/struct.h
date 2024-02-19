#ifndef __BYTES_STRUCT_H__
#define __BYTES_STRUCT_H__

#include "header.h"

typedef byte_vec_t evm_bytes_t;

evm_bytes_t bytes_init();
evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t i, uint64_t size);

#endif
