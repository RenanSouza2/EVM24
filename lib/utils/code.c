#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void uint64_display(uint64_t u)
{
    for(int i=7; i>=0; i--)
        printf("%02hx", ((uchar_t*)&u)[i]);
}



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
    printf("\n\t0x");uint64_display(i1);
    printf("\n\t0x");uint64_display(i2);
    printf("\n");
    printf("\n\tUINT64 ASSERTION ERROR\t\t");
    return false;
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
