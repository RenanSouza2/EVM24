#ifndef __RLP_DEBUG_H__
#define __RLP_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

void rlp_display(evm_rlp_t r);

evm_rlp_t rlp_init_immed(uint64_t type, ...);
evm_rlp_t rlp_init_immed_variadic(uint64_t type, va_list *args);

byte_vec_t rlp_encode_immed(uint64_t type, ...);
evm_rlp_t rlp_decode_immed(char str[]);

#endif

evm_rlp_vec_t rlp_vec_init(uint64_t size);
evm_rlp_t rlp_decode_rec(uint64_p ptr, byte_p in);
bool rlp_decode(evm_rlp_p r, byte_vec_p b);

#endif