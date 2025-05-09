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
    CLU_HANDLER_IS_SAFE(b->arr);

    return pos < b->size ? b->arr[pos] : 0;
}

word_t bytes_get_word(evm_bytes_p b, uint64_t pos)
{
    CLU_HANDLER_IS_SAFE(b->arr);

    word_t w = W1(0);
    for(int i=0; i<32; i++)
    {
        byte_t u = bytes_get_byte(b, pos+i);
        word_set_byte(&w, i, u);
    }
    return w;
}

evm_bytes_t bytes_get_bytes(evm_bytes_p b, uint64_t pos, uint64_t size)
{
    CLU_HANDLER_IS_SAFE(b->arr);

    if(size == 0)
        return byte_vec_init_zero();

    evm_bytes_t b_out = byte_vec_init(size);
    for(uint64_t i = 0; i < size; i++)
        b_out.arr[i] = bytes_get_byte(b, pos+i);

    return b_out;
}
