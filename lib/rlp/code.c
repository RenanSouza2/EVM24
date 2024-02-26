#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../utils/debug.h"



evm_rlp_t rlp_init_immed(uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    return rlp_init_immed_variadic(type, &args);

}

evm_rlp_t rlp_init_immed_variadic(uint64_t type, va_list *args)
{
    switch (type)
    {
        case BYTE:;
        char *str = va_arg(*args, char*);
        return (evm_rlp_t){BYTE, {.b = byte_vec_init_immed(str)}};
    
        case LIST:;
        uint64_t size = va_arg(*args, uint64_t);
        evm_rlp_t r = (evm_rlp_t){LIST, {.r = rlp_vec_init(size)}};
        for(uint64_t i=0; i<size; i++)
        {
            uint64_t type_2 = va_arg(*args, uint64_t);
            r.vec.r.v[i] = rlp_init_immed_variadic(type_2, args);
        }
        return r;
    }
    assert(false);
}



byte_vec_t rlp_encode_immed(uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    evm_rlp_t r = rlp_init_immed_variadic(type, &args);
    return rlp_encode(&r);
}

uint64_t rlp_decode_immed(evm_rlp_p r, char str[])
{
    byte_vec_t b = byte_vec_init_immed(str);

    uint64_t res = rlp_decode(r, &b);
    byte_vec_free(&b);
    return res;
}



bool rlp_test_immed(evm_rlp_t r, uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    evm_rlp_t r_exp = rlp_init_immed_variadic(type, &args);
    bool res = rlp_test(r, r_exp);
    // rlp_free(&r_exp);
    return res;
}

bool rlp_test(evm_rlp_t r, evm_rlp_t r_exp)
{
    if(!byte_test(r.type, r_exp.type))
    {
        printf("\n\tRLP ASSERTION ERROR | TYPE");
        return false;
    }

    switch (r.type)
    {
        case BYTE:
        if(!byte_vec_test(r.vec.b, r_exp.vec.b))
        {
            printf("\n\tRLP ASSERTION ERROR | BYTE VEC");
            return false;
        }
        return true;
    
        case LIST:
        if(!rlp_vec_test(r.vec.r, r_exp.vec.r))
        {
            printf("\n\tRLP ASSERTION ERROR | BYTE VEC");
            return false;
        }
        return true;
    }

    printf("\n\n\tRLP ASSERTION ERROR | INVALID TYPE");
    return false;
}

bool rlp_vec_test(evm_rlp_vec_t r, evm_rlp_vec_t r_exp)
{
    if(!byte_test(r.size, r_exp.size))
    {
        printf("\n\tRLP VECTOR ASSERTION ERROR | SIZE");
        return false;
    }
    
    for(uint64_t i=0; i<r.size; i++)
        if(!rlp_test(r.v[i], r_exp.v[i]))
        {
            printf("\n\tRLP VECTOR ASSERTION ERROR | ITEM | " U64P, i);
            return false;
        }

    return true;
}

#endif



evm_rlp_t rlp_init(uint64_t type, uint64_t size)
{
    switch (type)
    {
        case BYTE: return (evm_rlp_t){type, {.b = byte_vec_init(size)}};
        case LIST: return (evm_rlp_t){type, {.r = rlp_vec_init(size)}};
    }
    assert(false)
}

evm_rlp_vec_t rlp_vec_init(uint64_t size)
{
    if(size == 0) return (evm_rlp_vec_t){0, NULL};
    evm_rlp_p v = calloc(size, sizeof(evm_rlp_t));
    assert(v);

    return (evm_rlp_vec_t){size, v};
}



void rlp_free(evm_rlp_p r)
{
    switch (r->type)
    {
        case BYTE:
        byte_vec_free(&r->vec.b);
        return;
    
        case LIST:
        rlp_vec_free(&r->vec.r);
        return;
    }
    assert(false);
}

void rlp_vec_free(evm_rlp_vec_p r)
{
    if(r->v) free(r->v);
}

void rlp_vec_free_full(evm_rlp_vec_p r)
{
    for(uint64_t i=0; i<r->size; i++)
        rlp_free(&r->v[i]);
    if(r->v) free(r->v);
}



byte_vec_t rlp_encode(evm_rlp_p r)
{
    switch (r->type)
    {
        case BYTE:;
        byte_vec_t b = r->vec.b;
        if(b.size == 1)
        if(b.v[0] < 128)
            return b;
        
        if(b.size < 56)
        {
            byte_vec_t b_size = byte_vec_init_uint64(128 + b.size);
            return byte_vec_concat(&b_size, &b);
        }

        byte_vec_t b_size_2 = byte_vec_init_uint64(b.size);
        byte_vec_t b_size_1 = byte_vec_init_uint64(183 + b_size_2.size);
        b_size_1 = byte_vec_concat(&b_size_1, &b_size_2);
        return byte_vec_concat(&b_size_1, &b);
    
        case LIST:;
        evm_rlp_vec_t _r = r->vec.r;
        b = byte_vec_init_zero();
        for(int i=0; i<_r.size; i++)
        {
            byte_vec_t _b = rlp_encode(&_r.v[i]);
            b = byte_vec_concat(&b, &_b);
        }
        rlp_vec_free(&_r);

        if(b.size < 56)
        {
            byte_vec_t b_size = byte_vec_init_uint64(192 + b.size);
            return byte_vec_concat(&b_size, &b);
        }

        b_size_2 = byte_vec_init_uint64(b.size);
        b_size_1 = byte_vec_init_uint64(247 + b_size_2.size);
        b_size_1 = byte_vec_concat(&b_size_1, &b_size_2);
        return byte_vec_concat(&b_size_1, &b);
    }
    assert(false);
}



uint64_t rlp_get_size_b(uint64_p size_size, uint64_p size_bytes, byte_p b, uint64_t size)
{
    byte_t b0 = b[0];
    if(b0 < 128) 
    {
        *size_size = 0;
        *size_bytes = 1;
        return 0;
    }

    if(b0 < 184)
    {
        if(b0 - 127 > size) return 1;

        *size_size = 1;
        *size_bytes = b0 - 128;
        return 0;
    }

    uint64_t size_1 = b0 - 183;
    if(size_1 + 1 > size) return 2;
    
    uint64_t _size_bytes = uint64_init_byte(size_1, &b[1]);
    if(_size_bytes < 56) return 3;
    if(1 + size_1 + _size_bytes > size) return 4;

    *size_size = 1 + size_1;
    *size_bytes = _size_bytes;
    return 0;
}

uint64_t rlp_get_size_l(uint64_p size_size, uint64_p size_list, byte_p b, uint64_t size)
{
    byte_t b0 = b[0];
    if(b0 < 248)
    {
        if(b0 - 191 > size) return 1;

        *size_size = 1;
        *size_list = b0 - 192;
        return 0;
    }

    uint64_t size_1 = b0 - 247;
    if(size_1 + 1 > size) return 2;

    uint64_t _size_list = uint64_init_byte(size_1, &b[1]);
    if(_size_list < 56) return 3;
    if(1 + size_1 + _size_list > size) return 4;

    *size_size = 1 + size_1;
    *size_list = _size_list;
    return 0;
}

uint64_t rlp_get_size(uint64_p size_rlp, byte_p b, uint64_t size)
{
    if(size == 0) return 1;

    uint64_t size_size, _size_rlp;
    if(b[0] < 192) ERR(rlp_get_size_b(&size_size, &_size_rlp, b, size), 2)
    else           ERR(rlp_get_size_l(&size_size, &_size_rlp, b, size), 3)
    *size_rlp = size_size + _size_rlp;

    return 0;
}

uint64_t rlp_get_sizes_rec(uint64_vec_p sizes_r1, byte_p b, uint64_t size_list, uint64_t cnt)
{
    if(size_list == 0)
    {
        *sizes_r1 = uint64_vec_init(cnt);
        return 0;
    }

    uint64_t size_r1;
    ERR(rlp_get_size(&size_r1, b, size_list), 1);
    assert(size_r1 <= size_list);

    ERR1(rlp_get_sizes_rec(sizes_r1, &b[size_r1], size_list - size_r1, cnt));
    sizes_r1->v[cnt] = size_r1;
    return 0;
}

uint64_t rlp_get_sizes(uint64_vec_p sizes_r1, byte_p b, uint64_t size_list)
{
    uint64_vec_t _sizes_r1;
    ERR1(rlp_get_sizes_rec(&_sizes_r1, b, size_list, 0))

    *sizes_r1 = _sizes_r1;
    return 0;
}



evm_rlp_t rlp_decode_rec_b(byte_p in, uint64_t size_bytes)
{
    evm_rlp_t r = rlp_init(BYTE, size_bytes);
    memcpy(r.vec.b.v, in, size_bytes);
    return r;
}

uint64_t rlp_decode_rec_l(evm_rlp_p r, byte_p b, uint64_t size_list)
{
    uint64_vec_t r1_sizes;
    ERR(rlp_get_sizes(&r1_sizes, b, size_list), 1)

    evm_rlp_vec_t r0_vec = rlp_vec_init(r1_sizes.size);
    for(uint64_t i=0, ptr=0; i<r1_sizes.size; ptr += r1_sizes.v[i], i++)
    {
        evm_rlp_t r1;
        TRY(rlp_decode_rec(&r1, &b[ptr], r1_sizes.v[i]))
        {
            r0_vec.size = i;
            rlp_vec_free(&r0_vec);
            uint64_vec_free(&r1_sizes);
            return ERR_CONCAT(res, 2);
        }
        CATCH

        r0_vec.v[i] = r1;
    }

    
    uint64_vec_free(&r1_sizes);
    *r = (evm_rlp_t){LIST, {.r = r0_vec}};
    return 0;
}



uint64_t rlp_decode_rec(evm_rlp_p r, byte_p b, uint64_t size_rlp)
{
    if(size_rlp == 0) return 1;

    byte_t b0 = b[0];
    if(b0 < 192)
    {
        uint64_t size_size, size_bytes;
        ERR(rlp_get_size_b(&size_size, &size_bytes, b, size_rlp), 2);
        
        *r = rlp_decode_rec_b(&b[size_size], size_bytes);
        return 0;
    }

    uint64_t size_size, size_list;
    ERR(rlp_get_size_l(&size_size, &size_list, b, size_rlp), 3);
    ERR(rlp_decode_rec_l(r, &b[size_size], size_list), 4);
    
    return 0;
}

uint64_t rlp_decode(evm_rlp_p r, byte_vec_p b)
{
    evm_rlp_t _r;
    uint64_t size_list;

    ERR(rlp_get_size(&size_list, b->v, b->size), 1);
    assert(size_list <= b->size);
    if(size_list < b->size) return 2;

    ERR(rlp_decode_rec(&_r, b->v, b->size), 3);

    *r = _r;
    return 0;
}
