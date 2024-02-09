#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../bytes/debug.h"

mem_t mem_init_immed(char str[])
{
    mem_t m = bytes_init_immed(str);
    mem_expand(&m, m.size);
    return m;
}

bool mem_immed(mem_t m, char str[])
{
    mem_t m_exp = mem_init_immed(str);
    return bytes(m, m_exp);
}

#endif



mem_t mem_init()
{
    return (mem_t){NULL, 0};
}

void mem_free(mem_t m)
{
    bytes_free(&m);
}



int mem_get_size(mem_p m) // TODO: test
{
    return m->size >> 5;
}

int mem_dry_run(mem_p m, int i) // TODO: test
{
    int max = i + 0x1f;
    int m_size = m->size;
    return (max > m_size ? max : m_size) >> 5;
}

void mem_expand(mem_p m, int i)
{
    int max = (i + 0x1f) & ~0x1f;
    bytes_expand(m, max);
}



word_t mem_get_word(mem_p m, int i)
{
    mem_expand(m, i+32);
    return bytes_get_word(m, i);   
}

void mem_set_word(mem_p m, int i, word_p w)
{
    mem_expand(m, i+32);
    bytes_set_word(m, i, w);
}

void mem_set_byte(mem_p m, int i, uchar u)
{
    mem_expand(m, i+1);
    bytes_set_byte(m, i, u);
}