#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"
#include "../../utils/clu/bin/header.h"
#include "../utils/types.h"

uchar cton(uchar c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    assert(false);
}

void bytes_display(bytes_t b)
{
    printf("\n\nbytes (%d):", b.size);
    printf("\n");
    for(int i=0; i< b.size >> 5; i++)
    {
        printf("\n\t0x");
        for(int j=0; j<32; j++)
            printf("%02x", b.v[(i << 5) + j]);
    }
    if(b.size % 32)
    {
        int i= b.size & ~31;
        printf("\n\t0x");
        for(int j=i; j<b.size; j++)
            printf("%02x", b.v[j]);
    }
    printf("\n");
}

bytes_t bytes_init_immed(char str[])
{
    int len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');

    if(len == 2) return bytes_init(NULL, 0);

    int size = len / 2 - 1;
    uchar *b = malloc(size);
    for(int i=0; i<size; i++)
        b[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return bytes_init(b, size);
}
