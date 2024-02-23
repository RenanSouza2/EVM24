#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"


#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../utils/debug.h"



void rlp_display_rec(evm_rlp_t r, int cnt)
{
    printf("\nType: %s", r.type ? "str" : "bytes");
    
    switch (r.type)
    {
        case BYTE:
        byte_vec_display(r.vec.b);
        break;
    
        case LIST:
        printf("\n%*scount: " U64P, cnt, "\t", r.vec.r.size);
        for(int i=0; i<r.vec.r.size; i++)
        {
            printf("\n\n%*si: %d", cnt, "\t", i);
            rlp_display_rec(r.vec.r.v[i], cnt + 1);
        }
        break;
    }
}

void rlp_display(evm_rlp_t r) // TODO work here
{
    rlp_display_rec(r, 0);
}



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

evm_rlp_t rlp_decode_immed(char str[])
{
    byte_vec_t b = byte_vec_init_immed(str);

    evm_rlp_t r;
    assert(rlp_decode(&r, &b));
    byte_vec_free(&b);
    return r;
}



bool rlp_test_immed(evm_rlp_t r, uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    evm_rlp_t r_exp = rlp_init_immed_variadic(type, &args);
    bool res = rlp_test(r, r_exp);
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

    rlp_vec_free(&r_exp);
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
        for(uint64_t i=0; i<r->vec.r.size; i++)
            rlp_free(&r->vec.r.v[i]);
        rlp_vec_free(&r->vec.r);
        return;
    }
    assert(false);
}

void rlp_vec_free(evm_rlp_vec_p r)
{
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
    if(size) return rlp_decode_rec_s_rec(in, size, 0);
    return (evm_rlp_t){LIST, {.r = rlp_vec_init(0)}};
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

    uint64_t size, ptr_size;
    size = rlp_get_size_s(&ptr_size, in, b0);

    *ptr = ptr_size + size;
    return rlp_decode_rec_s(&in[ptr_size], size);
}

bool rlp_decode_validate(byte_vec_p b)
{
    byte_t b0 = b->v[0];
    uint64_t ptr_size, size;
    if(b0 < 192) size = rlp_get_size_b(&ptr_size, b->v, b0);
    else         size = rlp_get_size_s(&ptr_size, b->v, b0);
    return b->size == ptr_size + size;
}

bool rlp_decode(evm_rlp_p r, byte_vec_p b)
{
    if(!rlp_decode_validate(b)) return false;

    uint64_t ptr;
    *r = rlp_decode_rec(&ptr, b->v);
    return true;
}
