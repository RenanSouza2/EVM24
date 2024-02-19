#include "string.h"

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
        case AAAALIST: r.vec.r = rlp_vec_init(size);
        default: assert(false);
    }
    return r;
}



void rlp_decode(byte_p in)
{
    printf("%d", in[0]);

    if(in[0] < 128)
    {
        evm_rlp_t r = rlp_init(BYTE, 1);
        r.vec.b.v[0] = in[0];
        // return r;
    }

    if(in[0] < 184)
    {
        uint64_t size = in[0] - 128;
        evm_rlp_t r = rlp_init(BYTE, size);
        memcpy(r.vec.b.v, &in[1], size);
        // return r;
    }

    if(in[0] < 192)
    {
        uint64_t size_1 = in[0] - 183;
        uint64_t size_2 = 0;
        ((byte_p)&size_2)[0] = 0xff;
        ((byte_p)&size_2)[1] = 0xEE;
        printf("\n%lx", size_1);
        printf("\n%lx", size_2);
    }
}
