#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include <string.h>
#include <assert.h>

#include "../../utils/clu/bin/header.h"

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

#endif

bytes_t bytes_init(uchar b[], int size)
{
    return (bytes_t){b, size};
}

void bytes_free(bytes_t b)
{
    if(b.v) free(b.v);
}