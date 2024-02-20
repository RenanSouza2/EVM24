#ifndef __RLP_DEBUG_H__
#define __RLP_DEBUG_H__

#include "struct.h"

#ifdef DEBUG
#endif

evm_rlp_t rlp_decode_rec(uint64_p ptr, byte_p in);
bool rlp_decode(evm_rlp_p r, byte_vec_p b);

#endif
