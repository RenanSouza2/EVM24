#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"



uint64_vec_t uint64_vec_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    uint64_vec_t vec = uint64_vec_init(n);
    for(uint64_t i=0; i<n; i++)
        vec.v[i] = va_arg(args, uint64_t);

    return vec;
}



bool uchar_test(uchar_t u1, uchar_t u2)
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

bool uint64_vec_test_immed(uint64_vec_t vec, uint64_t n, ...)
{
    if(!uint64_test(vec.size, n))
    {
        printf("\n\tJUMPDEST TEST ASSERTION ERROR | COUNT");
        return false;
    }

    va_list args;
    va_start(args, n);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t jumpdest = va_arg(args, uint64_t);
        if(!uint64_test(vec.v[i], jumpdest))
        {
            printf("\n\tJUMPDEST TEST ASSERTION ERROR | JUMPDEST | " U64P, i);
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



uint64_t uint128_to_uint64(uint128_t res)
{
    return (res >> 64) ? UINT64_MAX : (uint64_t) res;
}



uint64_vec_t uint64_vec_init(uint64_t size)
{
    uint64_p v = calloc(size, sizeof(uint64_t));
    assert(v);

    return (uint64_vec_t){size, v};
}

#define VEC_FREE(TYPE)                      \
    void TYPE##_vec_free(TYPE##_vec_p vec)  \
    {                                       \
        if(vec->v) free(vec->v);            \
    }

VEC_FREE(uchar)
VEC_FREE(uint64)

uint64_t uint64_vec_binary_search(uint64_vec_p vec, uint64_t v)
{
    assert(vec->size);

    uint64_t min = 0;
    for(uint64_t max = vec->size; max - min > 1; )
    {
        uint64_t mid = (min + max) >> 1;
        uint64_t _v = vec->v[mid];
        if(_v == v) return mid;

        if(vec->v[mid] > v) max = mid;
        else                min = mid;    
    }
    return min;
}

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v) // TODO test
{
    if(vec->size == 0) return false;

    uint64_t pos = uint64_vec_binary_search(vec, v);
    return vec->v[pos] == v;
}
