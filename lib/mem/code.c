#include <stdio.h>
#include <stdlib.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../bytes/debug.h"
#include "../gas/header.h"

evm_mem_t mem_init_immed(char str[])
{
    evm_mem_t m = bytes_init_immed(str);
    mem_expand(&m, m.size);
    return m;
}

bool mem_test_immed(evm_mem_t m, char str[])
{
    evm_mem_t m_exp = mem_init_immed(str);
    if(bytes_test(m, m_exp)) return true;

    printf("\n\tMEM ASSERTTION ERROR | BYTES ASSERTTION ERROR\t\t");
    return false;
}

#endif



evm_mem_t mem_init()
{
    return (evm_mem_t){NULL, 0};
}

void mem_free(evm_mem_t m)
{
    bytes_free(&m);
}



int mem_dry_run(evm_mem_p m, int i)
{
    int max = i + 0x1f;
    int m_size_aft = (max > m->size ? max : m->size) >> 5;

    int m_size_bef = m->size >> 5;
    int gas_expansion = (m_size_bef == m_size_aft) ? 0 : gas_mem(m_size_aft) - gas_mem(m_size_bef);
    return G_very_low + gas_expansion;
}

void mem_expand(evm_mem_p m, int i)
{
    int max = (i + 0x1f) & ~0x1f;
    bytes_expand(m, max);
}



evm_word_t mem_get_word(evm_mem_p m, int i)
{
    mem_expand(m, i+32);
    return bytes_get_word(m, i);
}

void mem_set_word(evm_mem_p m, int i, evm_word_p w)
{
    mem_expand(m, i+32);
    bytes_set_word(m, i, w);
}

void mem_set_byte(evm_mem_p m, int i, uchar u)
{
    mem_expand(m, i+1);
    bytes_set_byte(m, i, u);
}
