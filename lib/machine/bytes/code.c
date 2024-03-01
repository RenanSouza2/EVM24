#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../../utils/assert.h"



#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#endif



byte_t bytes_get_byte(evm_bytes_p b, uint64_t pos)
{
    assert(pos >= 0);
    return pos < b->size ? b->v[pos] : 0;
}

word_t bytes_get_word(evm_bytes_p b, uint64_t pos)
{
    word_t w = word_init();
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
