#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void uint64_display(uint64_t u)
{
    for(int i=7; i>=0; i--)
        printf("%02hx", ((uchar*)&u)[i]);
}

bool int_test(int i1, int i2)
{
    if(i1 == i2) return true;

    printf("\n\n\t\tINT ASSERTION ERROR | %d %d\t\t", i1, i2);
    return false;
}

bool uint64(uint64_t i1, uint64_t i2)
{
    if(i1 == i2) return true;

    printf("\n");
    printf("\n\t\t0x");uint64_display(i1);
    printf("\n\t\t0x");uint64_display(i2);
    printf("\n");
    printf("\n\t\tUINT64 ASSERTION ERROR\t\t");
    return false;
}

#endif
