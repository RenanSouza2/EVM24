#include <stdio.h>
#include <stdlib.h>

#include "debug.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../bytes/debug.h"
#include "../gas/header.h"
#include "../utils/debug.h"



evm_mem_t mem_init_immed(int n, ...)
{
    va_list args;
    va_start(args, n);
    return mem_init_immed_variadic(n, &args);
}

evm_mem_t mem_init_immed_variadic(int n, va_list *arg)
{
    evm_mem_t m = mem_init();
    bytes_expand(&m, n << 5);
    for(int i=0; i<n; i++)
    {
        evm_word_t w = va_arg(*arg, evm_word_t);
        for(int j=0; j<32; j++)
            m.v[(i << 5) + j] = word_get_byte(&w, 31 - j);
    }
    return m;
}



bool mem_test_immed(evm_mem_t m, int n, ...)
{
    va_list args;
    va_start(args, n);
    return mem_test_variadic(m, n, &args);
}

bool mem_test_variadic(evm_mem_t m, int n, va_list *args)
{
    if(!int_test(m.size, n << 5))
    {
        printf("\n\tMEM ASSERTTION ERROR | LENGTH\t\t");
        return false;
    }

    evm_mem_t m_exp = mem_init_immed_variadic(n, args);
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
