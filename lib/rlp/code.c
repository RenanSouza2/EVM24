#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"


#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



evm_rlp_vec_t rlp_vec_init(uint64_t size)
{
    if(size == 0) return (evm_rlp_vec_t){0, NULL};
    evm_rlp_p v = calloc(size, sizeof(evm_rlp_t));
    assert(v);

    return (evm_rlp_vec_t){size, v};
}

evm_rlp_t rlp_init(uint64_t type, uint64_t size)
{
    evm_rlp_t r;
    r.type = type;
    switch (type)
    {
        case BYTE: r.vec.b = byte_vec_init(size);
        case LIST: r.vec.r = rlp_vec_init(size);
        default: assert(false);
    }
    return r;
}



uint64_t rlp_get_size_b(uint64_p ptr, byte_p in, byte_t b0)
{
    if(b0 < 128) 
    {
        *ptr = 0;
        return 1;
    }

    if(b0 < 184)
    {
        *ptr = 1;
        return b0 - 128;
    }

    uint64_t size_1 = b0 - 183;
    *ptr = 1 + size_1;
    return uint64_init_byte(size_1, &in[1]);
}

evm_rlp_t rlp_decode_rec_b(byte_p in, uint64_t size)
{
    evm_rlp_t r = rlp_init(BYTE, size);
    memcpy(r.vec.b.v, in, size);
    return r;
}



uint64_t rlp_get_size_s(uint64_p ptr, byte_p in, byte_t b0)
{
    if(b0 < 248)
    {
        *ptr = 1;
        return b0 - 192;
    }

    uint64_t size_1 = b0 - 247;
    *ptr = 1 + size_1;
    return uint64_init_byte(size_1, &in[1]);
}

evm_rlp_t rlp_decode_rec_s_rec(byte_p in, uint64_t size, int count)
{
    uint64_t ptr;
    evm_rlp_t r1 = rlp_decode_rec(&ptr, in);

    if(ptr == size)
    {
        evm_rlp_t r0 = rlp_init(LIST, count + 1);
        r0.vec.r.v[count] = r1;
        return r0;
    }

    evm_rlp_t r0 = rlp_decode_rec_s_rec(&in[ptr], size - ptr, count + 1);
    r0.vec.r.v[count] = r1;
    return r0;
}

evm_rlp_t rlp_decode_rec_s(byte_p in, uint64_t size)
{
    return rlp_decode_rec_s_rec(in, size, 0);
}



evm_rlp_t rlp_decode_rec(uint64_p ptr, byte_p in)
{
    byte_t b0 = in[0];
    if(b0 < 192)
    {
        uint64_t ptr_size;
        uint64_t size = rlp_get_size_b(&ptr_size, in, b0);
        *ptr = ptr_size + size;
        return rlp_decode_rec_b(&in[ptr_size], size);
    }

    uint64_t ptr_size;
    uint64_t size = rlp_get_size_s(&ptr_size, in, b0);
    *ptr = ptr_size + size;
    return rlp_decode_rec_s(&in[ptr_size], size);
}

bool rlp_decode_validate(byte_vec_p b)
{
    byte_t b0 = b->v[0];
    uint64_t ptr_size, size;
    if(b0 < 192) size = rlp_get_size_b(&ptr_size, b->v, b0);
    else         size = rlp_get_size_s(&ptr_size, b->v, b0);
    return b->size != ptr_size + size;
}

bool rlp_decode(evm_rlp_p r, byte_vec_p b)
{
    if(!rlp_decode_validate(b)) return false;

    uint64_t ptr;
    *r = rlp_decode_rec(&ptr, b->v);
    return true;
}
