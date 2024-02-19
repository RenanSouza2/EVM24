#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../../utils/assert.h"



#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"
#include "../../utils/debug.h"



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

#endif



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
    if(size == 0) return byte_vec_init_zero();
    
    byte_t *v = malloc(size);
    assert(v);
    for(uint64_t i = 0; i < size; i++)
        v[i] = bytes_get_byte(b, pos+i);
    
    return (evm_bytes_t){size, v};
}
