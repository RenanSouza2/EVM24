#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include "debug.c"

#endif



bytes_t bytes_init(uchar b[], int size)
{
    return (bytes_t){b, size};
}

void bytes_free(bytes_t b)
{
    if(b.v) free(b.v);
}



uchar bytes_get_byte(bytes_p b, int i)
{
    assert(i >= 0);
    return i < b->size ? b->v[i] : 0;
}

void bytes_expand(bytes_p b, int size)
{
    if(size <= b->size) return;

    int size_prev = b->size;
    b->size = size;
    b->v = realloc(b->v, size);
    memset(&b->v[size_prev], 0, size - size_prev);
}

void bytes_set_byte(bytes_p b, int i, uchar u)
{
    bytes_expand(b, i+1);
    b->v[i] = u;
}

word_t bytes_get_word(bytes_p b, int i)
{
    word_t w = word_init_zero();
    for(int _i = 0; _i < 32; _i++)
    {
        uchar u = bytes_get_byte(b, i+_i);
        word_set_byte(&w, 31-_i, u);
    }
    return w;
}

void bytes_set_word(bytes_p b, int i, word_p w)
{
    bytes_expand(b, i+32);
    for(int _i=0; _i < 32; _i++)
        b->v[i + _i] = word_get_byte(w, 31-_i);
}

bytes_t bytes_get_bytes(bytes_p b, int i, int size)
{
    if(size == 0) return (bytes_t){NULL, 0};
    
    uchar *v = calloc(size, 1);
    assert(v);
    for(int _i = 0; _i < size; _i++)
        v[_i] = bytes_get_byte(b, i+_i);
    
    return (bytes_t){v, size};
}
