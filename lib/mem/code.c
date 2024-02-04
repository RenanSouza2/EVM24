#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



mem_t mem_init()
{
    return (mem_t){NULL, 0};
}

void mem_free(mem_t m)
{
    bytes_free(&m);
}



word_t mem_get_word(mem_p m, int i)
{
    bytes_expand(m, i+32);
    return bytes_get_word(m, i);   
}

void mem_set_word(mem_p m, int i, word_p w)
{
    bytes_set_word(m, i, w);
}

void mem_set_byte(mem_p m, int i, uchar u)
{
    bytes_set_byte(m, i, u);
}