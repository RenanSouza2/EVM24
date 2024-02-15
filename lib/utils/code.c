#include <stdlib.h>
#include <assert.h>

#include "debug.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"



bool uchar_test(uchar_t u1, uchar_t u2)
{
    if(u1 == u2) return true;

    printf("\n\n\tUCHAR ASSERTION ERROR | %d %d\t\t", u1, u2);
    return false;
}

bool int_test(int i1, int i2)
{
    if(i1 == i2) return true;

    printf("\n\n\tINT ASSERTION ERROR | %d %d\t\t", i1, i2);
    return false;
}

bool uint64_test(uint64_t i1, uint64_t i2)
{
    if(i1 == i2) return true;

    printf("\n");
    printf("\n\t0x" U64PX, i1);
    printf("\n\t0x" U64PX, i2);
    printf("\n");
    printf("\n\tUINT64 ASSERTION ERROR\t\t");
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
            printf("\n\tJUMPDEST TEST ASSERTION ERROR | JUMPDEST | %llu", i);
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



uint64_t uint128_to_uint64(uint128_t res) // TODO test
{
    return (res >> 64) ? UINT64_MAX : (uint64_t) res;
}



uint64_vec_t uint64_vec_init(uint64_t size)
{
    uint64_p v = calloc(size, sizeof(uint64_t));
    assert(v);

    return (uint64_vec_t){size, v};
}

void uint64_vec_free(uint64_vec_t vec)
{
    if(vec.v) free(vec.v);
}

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v) // TODO test
{
    for(uint64_t min = 0, max = vec->size; max - min > 1;)
    {
        uint64_t mid = (min + max) >> 1;
        uint64_t _v = vec->v[mid];
        if(_v == v) return true;

        if(vec->v[mid] > v) max = mid;
        else                min = mid;    
    }
    return false;
}
