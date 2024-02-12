#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../utils/debug.h"



uchar_t cton(uchar_t c)
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
    printf("\n\nbytes (%llu):", b.size);
    printf("\n");
    for(uint64_t i=0; i< b.size >> 5; i++)
    {
        printf("\n\t0x");
        for(uint64_t j=0; j<32; j++)
            printf("%02x", b.v[(i << 5) + j]);
    }
    if(b.size % 32)
    {
        uint64_t i = b.size & ~31;
        printf("\n\t0x");
        for(uint64_t j=i; j<b.size; j++)
            printf("%02x", b.v[j]);
    }
    printf("\n");
}

evm_bytes_t bytes_init_immed(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');

    if(len == 2) return (evm_bytes_t){NULL, 0};

    uint64_t size = len / 2 - 1;
    uchar_t *b = malloc(size);
    for(uint64_t i=0; i<size; i++)
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

    for(uint64_t i=0; i<b.size; i++)
    {
        if(!uchar_test(b.v[i], b_exp.v[i]))
        {
            printf("\n\tBYTES ASSERTION ERROR | BYTE | %lld\t\t", i);
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



uchar_t bytes_get_byte(evm_bytes_p b, uint64_t i)
{
    assert(i >= 0);
    return i < b->size ? b->v[i] : 0;
}

void bytes_expand(evm_bytes_p b, uint64_t size)
{
    if(size <= b->size) return;

    uint64_t size_prev = b->size;
    b->size = size;
    b->v = realloc(b->v, size);
    memset(&b->v[size_prev], 0, size - size_prev);
}

void bytes_set_byte(evm_bytes_p b, uint64_t i, uchar_t u)
{
    bytes_expand(b, i+1);
    b->v[i] = u;
}

evm_word_t bytes_get_word(evm_bytes_p b, uint64_t i)
{
    evm_word_t w = word_init();
    for(int _i=0; _i<32; _i++)
    {
        uchar_t u = bytes_get_byte(b, i+_i);
        word_set_byte(&w, 31-_i, u);
    }
    return w;
}

void bytes_set_word(evm_bytes_p b, uint64_t i, evm_word_p w)
{
    bytes_expand(b, i+32);
    for(int _i=0; _i<32; _i++)
        b->v[i + _i] = word_get_byte(w, 31-_i);
}

evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t i, uint64_t size)
{
    if(size == 0) return (evm_bytes_t){NULL, 0};
    
    uchar_t *v = calloc(size, 1);
    assert(v);
    for(uint64_t _i = 0; _i < size; _i++)
        v[_i] = bytes_get_byte(b, i+_i);
    
    return (evm_bytes_t){v, size};
}
