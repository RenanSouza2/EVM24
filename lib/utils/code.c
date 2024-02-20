#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include <stdarg.h>
#include <string.h>

#include "../../utils/clu/bin/header.h"



byte_t cton(byte_t c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    assert(false);
}

byte_vec_t byte_vec_init_immed(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');
    if(len == 2) return byte_vec_init(0);

    uint64_t size = len / 2 - 1;
    byte_t *b = malloc(size);
    for(uint64_t i=0; i<size; i++)
        b[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return (byte_vec_t){size, b};
}

uint64_vec_t uint64_vec_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    uint64_vec_t vec = uint64_vec_init(n);
    for(uint64_t i=0; i<n; i++)
        vec.v[i] = va_arg(args, uint64_t);

    return vec;
}

uint64_t uint64_init_byte_immed(char str[])
{
    byte_vec_t b = byte_vec_init_immed(str);
    uint64_t res = uint64_init_byte(b.size, b.v);
    byte_vec_free(&b);
    return res;
}



bool byte_test(byte_t u1, byte_t u2)
{
    if(u1 == u2) return true;

    printf("\n\n\tUCHAR ASSERTION ERROR | %d %d", u1, u2);
    return false;
}

bool int_test(int i1, int i2)
{
    if(i1 == i2) return true;

    printf("\n\n\tINT ASSERTION ERROR | %d %d", i1, i2);
    return false;
}

bool uint64_test(uint64_t i1, uint64_t i2)
{
    if(i1 == i2) return true;

    printf("\n");
    printf("\n\t0x" U64PX, i1);
    printf("\n\t0x" U64PX, i2);
    printf("\n");
    printf("\n\tUINT64 ASSERTION ERROR");
    return false;
}



bool byte_vec_test(byte_vec_t b, byte_vec_t b_exp)
{
    if(!int_test(b.size, b_exp.size)) 
    {
        printf("\n\tBYTE VEC ASSERTION ERROR | LENGTH");
        byte_vec_free(&b_exp);
        return false;
    }

    for(uint64_t i=0; i<b.size; i++)
    {
        if(!byte_test(b.v[i], b_exp.v[i]))
        {
            printf("\n\tBYTE VEC ASSERTION ERROR | BYTE | " U64P, i);
            byte_vec_free(&b_exp);
            return false;
        }
    }

    byte_vec_free(&b_exp);
    return true;
}

bool byte_vec_test_immed(byte_vec_t b, char str[])
{
    byte_vec_t b_exp = byte_vec_init_immed(str);
    return byte_vec_test(b, b_exp);
}

bool uint64_vec_test_immed(uint64_vec_t vec, uint64_t n, ...)
{
    if(!uint64_test(vec.size, n))
    {
        printf("\n\tUINT64 VEC TEST ASSERTION ERROR | COUNT");
        return false;
    }

    va_list args;
    va_start(args, n);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t jumpdest = va_arg(args, uint64_t);
        if(!uint64_test(vec.v[i], jumpdest))
        {
            printf("\n\tUINT64 VEC TEST ASSERTION ERROR | UINT64 | " U64P, i);
            return false;
        }
    }

    return true;
}

#endif



uint64_t uint64_add(uint64_t u1, uint64_t u2)
{
    uint64_t sum = u1 + u2;
    return sum < u1 ? UINT64_MAX : sum;
}

uint64_t uint64_set_byte(uint64_t u, int index, byte_t b)
{
    int offset = index << 3;
    return (u & ~((uint64_t)0xff << offset)) | ((uint64_t)b << offset);
}

uint64_t uint64_init_byte(uint64_t size, byte_p b)
{
    uint64_t u = 0;
    for(int i=0; i<size; i++)
        u = uint64_set_byte(u, i, b[size-1 - i]);
    return u;
}

uint64_t uint128_to_uint64(uint128_t res)
{
    return (res >> 64) ? UINT64_MAX : (uint64_t) res;
}



#define VEC_INIT(TYPE)                                  \
    TYPE##_vec_t TYPE##_vec_init(uint64_t size)         \
    {                                                   \
        if(size == 0) return (TYPE##_vec_t){0, NULL};   \
        TYPE##_p v = calloc(size, sizeof(TYPE##_t));    \
        assert(v);                                      \
        return (TYPE##_vec_t){size, v};                 \
    }

#define VEC_FREE(TYPE)                      \
    void TYPE##_vec_free(TYPE##_vec_p vec)  \
    {                                       \
        if(vec->v) free(vec->v);            \
    }

byte_vec_t byte_vec_init_zero()
{
    return (byte_vec_t){0, NULL};
}

VEC_INIT(byte);
VEC_INIT(uint64);

VEC_FREE(byte);
VEC_FREE(uint64);

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v)
{
    if(vec->size == 0) return false;

    uint64_t min = 0;
    for(uint64_t max = vec->size; max - min > 1; )
    {
        uint64_t mid = (min + max) >> 1;
        uint64_t _v = vec->v[mid];
        if(_v == v) return true;

        if(_v >  v) max = mid;
        else        min = mid;    
    }
    return vec->v[min] == v;
}
