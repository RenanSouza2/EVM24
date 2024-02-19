#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include <string.h>

#include "../../utils/clu/bin/header.h"
#include "../utils/debug.h"



byte_t cton(byte_t c)
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
    printf("\n\nbytes (" U64P "):", b.size);
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

    if(len == 2) return bytes_init();

    uint64_t size = len / 2 - 1;
    byte_t *b = malloc(size);
    for(uint64_t i=0; i<size; i++)
        b[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return (evm_bytes_t){size, b};
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
        printf("\n\n\tBYTES ASSERTION ERROR | LENGTH");
        bytes_free(&b_exp);
        return false;
    }

    for(uint64_t i=0; i<b.size; i++)
    {
        if(!uchar_test(b.v[i], b_exp.v[i]))
        {
            printf("\n\tBYTES ASSERTION ERROR | BYTE | " U64P, i);
            bytes_free(&b_exp);
            return false;
        }
    }

    bytes_free(&b_exp);
    return true;
}

#endif



evm_bytes_t bytes_init()
{
    return (evm_bytes_t){0, NULL};
}

void bytes_free(evm_bytes_p b)
{
    byte_vec_free(b);
}



byte_t bytes_get_byte(evm_bytes_p b, uint64_t pos)
{
    assert(pos >= 0);
    return pos < b->size ? b->v[pos] : 0;
}

evm_word_t bytes_get_word(evm_bytes_p b, uint64_t pos)
{
    evm_word_t w = word_init();
    for(int i=0; i<32; i++)
    {
        byte_t u = bytes_get_byte(b, pos+i);
        word_set_byte(&w, 31-i, u);
    }
    return w;
}

evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t pos, uint64_t size)
{
    if(size == 0) return bytes_init();
    
    byte_t *v = malloc(size);
    assert(v);
    for(uint64_t i = 0; i < size; i++)
        v[i] = bytes_get_byte(b, pos+i);
    
    return (evm_bytes_t){size, v};
}
