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
        byte_vec_t b = byte_vec_init_immed(str);
        return rlp_init_byte(&b);
    
        case LIST:;
        uint64_t size = va_arg(*args, uint64_t);
        evm_rlp_vec_t r = rlp_vec_init(size);
        for(uint64_t i=0; i<size; i++)
        {
            uint64_t r1_type = va_arg(*args, uint64_t);
            r.v[i] = rlp_init_immed_variadic(r1_type, args);
        }
        
        return rlp_init_list(&r);
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
    return rlp_test(r, r_exp);;
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
            printf("\n\tRLP ASSERTION ERROR | LIST VEC");
            return false;
        }
        return true;
    }

    printf("\n\n\tRLP ASSERTION ERROR | INVALID TYPE");
    return false;
}

bool rlp_vec_test(evm_rlp_vec_t r, evm_rlp_vec_t r_exp)
{
    if(!uint64_test(r.size, r_exp.size))
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



evm_rlp_t rlp_init_byte(byte_vec_p b)
{
    return (evm_rlp_t){BYTE, {.b = *b}};
}

evm_rlp_t rlp_init_list(evm_rlp_vec_p r)
{
    return (evm_rlp_t){LIST, {.r = *r}};
}

evm_rlp_vec_t rlp_vec_init(uint64_t size)
{
    if(size == 0) return (evm_rlp_vec_t){0, NULL};
    evm_rlp_p v = calloc(size, sizeof(evm_rlp_t));
    assert(v);

    return (evm_rlp_vec_t){size, v};
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

void rlp_free(evm_rlp_p r)
{
    switch (r->type)
    {
        case BYTE:
        byte_vec_free(&r->vec.b);
        return;
    
        case LIST:
        rlp_vec_free_full(&r->vec.r);
        return;
    }
    assert(false);
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
        for(uint64_t i=0; i<_r.size; i++)
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



uint64_t rlp_get_size_1(uint64_p head_size, uint64_p body_size, uint64_t _body_size, uint64_t size)
{
    uint64_t _head_size = 1;
    if(_head_size + _body_size > size) return 1;

    *head_size = _head_size;
    *body_size = _body_size;
    return 0;
}

uint64_t rlp_get_size_2(uint64_p head_size, uint64_p body_size, byte_p b, uint64_t _size_size, uint64_t size)
{
    uint64_t _head_size = 1 + _size_size;
    if(_head_size > size) return 1;
    
    uint64_t _body_size = uint64_init_byte(&b[1], _size_size);
    if(_body_size < 56) return 2;
    if(_head_size + _body_size > size) return 3;

    *head_size = _head_size;
    *body_size = _body_size;
    return 0;
}

uint64_t rlp_get_size(uint64_p head_size, uint64_p body_size, byte_p b, uint64_t size)
{
    if(size == 0) return 1;

    byte_t b0 = b[0];
    if(b0 < 128) 
    {
        *head_size = 0;
        *body_size = 1;
        return 0;
    }

    if(b0 < 184) 
    {
        ERR(rlp_get_size_1(head_size, body_size, b0 - 128, size), 2)
        return 0;
    }
    
    if(b0 < 192)
    {
        ERR(rlp_get_size_2(head_size, body_size, b, b0 - 183, size), 3);
        return 0;
    }
    
    if(b0 < 248)
    {
        ERR(rlp_get_size_1(head_size, body_size, b0 - 192, size), 4);
        return 0;
    }

    ERR(rlp_get_size_2(head_size, body_size, b, b0 - 247, size), 5);
    return 0;
}

uint64_t rlp_get_sizes_rec(uint64_vec_p r1_sizes, byte_p b, uint64_t list_size, uint64_t cnt)
{
    if(list_size == 0)
    {
        *r1_sizes = uint64_vec_init(cnt << 1);
        return 0;
    }

    uint64_t head_size, body_size;
    ERR(rlp_get_size(&head_size, &body_size, b, list_size), 1);

    uint64_t r1_size = head_size + body_size;
    assert(r1_size <= list_size);
    ERR(rlp_get_sizes_rec(r1_sizes, &b[r1_size], list_size - r1_size, cnt + 1), 0);

    uint64_t index = cnt << 1;
    r1_sizes->v[index    ] = head_size;
    r1_sizes->v[index + 1] = body_size;
    return 0;
}

uint64_t rlp_get_sizes(uint64_vec_p r1_sizes, byte_p b, uint64_t list_size)
{
    uint64_vec_t _r1_sizes;
    ERR(rlp_get_sizes_rec(&_r1_sizes, b, list_size, 0), 0)

    *r1_sizes = _r1_sizes;
    return 0;
}



evm_rlp_t rlp_decode_rec_b(byte_p b, uint64_t body_size)
{
    byte_vec_t _b = byte_vec_init(body_size);
    memcpy(_b.v, b, body_size);

    return rlp_init_byte(&_b);
}

uint64_t rlp_decode_rec_l(evm_rlp_p r, byte_p b, uint64_t list_size)
{
    uint64_vec_t r1_sizes;
    ERR(rlp_get_sizes(&r1_sizes, b, list_size), 1)

    uint64_t r_vec_size = r1_sizes.size >> 1;
    evm_rlp_vec_t r_vec = rlp_vec_init(r_vec_size);
    for(uint64_t i=0, ptr=0; i<r_vec_size; i++)
    {
        uint64_t index, head_size, body_size;
        index = i << 1;
        head_size = r1_sizes.v[index];
        body_size = r1_sizes.v[index + 1];

        evm_rlp_t r1;
        TRY(rlp_decode_rec(&r1, &b[ptr], head_size, body_size))
        {
            r_vec.size = i;
            rlp_vec_free_full(&r_vec);
            uint64_vec_free(&r1_sizes);
            return ERR_CONCAT(res, 2);
        }
        CATCH

        r_vec.v[i] = r1;
        ptr += head_size + body_size;
    }
    uint64_vec_free(&r1_sizes);

    *r = rlp_init_list(&r_vec);
    return 0;
}



uint64_t rlp_decode_rec(evm_rlp_p r, byte_p b, uint64_t head_size, uint64_t body_size)
{
    byte_t b0 = b[0];
    if(b0 < 192)
    {
        *r = rlp_decode_rec_b(&b[head_size], body_size);
        return 0;
    }

    ERR(rlp_decode_rec_l(r, &b[head_size], body_size), 1);
    return 0;
}

uint64_t rlp_decode(evm_rlp_p r, byte_vec_p b)
{
    uint64_t head_size, body_size;
    ERR(rlp_get_size(&head_size, &body_size, b->v, b->size), 1);

    uint64_t rlp_size = head_size + body_size;
    assert(rlp_size <= b->size);
    if(rlp_size < b->size) return 2;

    evm_rlp_t _r;
    ERR(rlp_decode_rec(&_r, b->v, head_size, body_size), 3);

    *r = _r;
    return 0;
}
