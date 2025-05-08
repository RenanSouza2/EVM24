#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../../mods/clu/header.h"
#include "../../../mods/macros/assert.h"

#include "../../utils/header.h"
#include "../../word/header.h"



#ifdef DEBUG
#endif



byte_t bytes_get_byte(evm_bytes_p b, uint64_t pos)
{
    return pos < b->size ? b->arr[pos] : 0;
}

word_t bytes_get_word(evm_bytes_p b, uint64_t pos)
{
    word_t w = W1(0);
    for(int i=0; i<32; i++)
    {
        byte_t u = bytes_get_byte(b, pos+i);
        word_set_byte(&w, 31-i, u);
    }
    return w;
}

evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t pos, uint64_t size)
{
    if(size == 0)
        return byte_vec_init_zero();

    byte_t *arr = malloc(size);
    assert(arr);
    for(uint64_t i = 0; i < size; i++)
        arr[i] = bytes_get_byte(b, pos+i);

    return (evm_bytes_t)
    {
        .size = size,
        .arr = arr
    };
}
