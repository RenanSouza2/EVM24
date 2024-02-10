#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../utils/debug.h"

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

void bytes_display(evm_bytes_t b)
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

evm_bytes_t bytes_init_immed(char str[])
{
    int len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');

    if(len == 2) return (evm_bytes_t){NULL, 0};

    int size = len / 2 - 1;
    uchar *b = malloc(size);
    for(int i=0; i<size; i++)
        b[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return (evm_bytes_t){b, size};
}

bool bytes_test_immed(evm_bytes_t b, char str[])
{
    evm_bytes_t b_exp = bytes_init_immed(str);
    return bytes_test(b, b_exp);
}

bool bytes_test(evm_bytes_t b, evm_bytes_t b_exp)
{
    if(!int_test(b.size, b_exp.size)) 
    {
        printf("\n\n\tBYTES ASSERTION ERROR | LENGTH\t\t");
        bytes_free(&b_exp);
        return false;
    }

    for(int i=0; i<b.size; i++)
    {
        if(!uchar_test(b.v[i], b_exp.v[i]))
        {
            printf("\n\n\tBYTES ASSERTION ERROR | BYTE | %d\t\t", i);
            bytes_free(&b_exp);
            return false;
        }
    }

    bytes_free(&b_exp);
    return true;
}

#endif



evm_bytes_t bytes_init_zero()
{
    return (evm_bytes_t){NULL, 0};
}

void bytes_free(evm_bytes_p b)
{
    if(b->v) free(b->v);
}



uchar bytes_get_byte(evm_bytes_p b, int i)
{
    assert(i >= 0);
    return i < b->size ? b->v[i] : 0;
}

void bytes_expand(evm_bytes_p b, int size)
{
    if(size <= b->size) return;

    int size_prev = b->size;
    b->size = size;
    b->v = realloc(b->v, size);
    memset(&b->v[size_prev], 0, size - size_prev);
}

void bytes_set_byte(evm_bytes_p b, int i, uchar u)
{
    bytes_expand(b, i+1);
    b->v[i] = u;
}

evm_word_t bytes_get_word(evm_bytes_p b, int i)
{
    evm_word_t w = word_init();
    for(int _i = 0; _i < 32; _i++)
    {
        uchar u = bytes_get_byte(b, i+_i);
        word_set_byte(&w, 31-_i, u);
    }
    return w;
}

void bytes_set_word(evm_bytes_p b, int i, evm_word_p w)
{
    bytes_expand(b, i+32);
    for(int _i=0; _i < 32; _i++)
        b->v[i + _i] = word_get_byte(w, 31-_i);
}

evm_bytes_t bytes_get_bytes(evm_bytes_p b, int i, int size)
{
    if(size == 0) return (evm_bytes_t){NULL, 0};
    
    uchar *v = calloc(size, 1);
    assert(v);
    for(int _i = 0; _i < size; _i++)
        v[_i] = bytes_get_byte(b, i+_i);
    
    return (evm_bytes_t){v, size};
}
