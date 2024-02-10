#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void uint64_display(uint64_t u)
{
    for(int i=7; i>=0; i--)
        printf("%02hx", ((uchar*)&u)[i]);
}



bool uchar_test(uchar u1, uchar u2)
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

void error_log(char str[])
{
    FILE *fp = fopen("error.log.txt", "w");
    assert(fp);

    fprintf(fp, "%s", str);
    fclose(fp);
}