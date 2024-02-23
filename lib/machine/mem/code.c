#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../../utils/assert.h"

#include "../gas/header.h"
#include "../bytes/struct.h"



#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"



evm_mem_t mem_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return mem_init_immed_variadic(n, &args);
}

evm_mem_t mem_init_immed_variadic(uint64_t n, va_list *arg)
{
    evm_mem_t m = byte_vec_init_zero();
    mem_expand(&m, n << 5);
    for(uint64_t i=0; i<n; i++)
    {
        evm_word_t w = va_arg(*arg, evm_word_t);
        for(int j=0; j<32; j++)
            m.v[(i << 5) + j] = word_get_byte(&w, 31 - j);
    }
    return m;
}



bool mem_test_immed(evm_mem_t m, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return mem_test_variadic(m, n, &args);
}

bool mem_test_variadic(evm_mem_t m, uint64_t n, va_list *args)
{
    if(!int_test(m.size, n << 5))
    {
        printf("\n\tMEM ASSERTTION ERROR | LENGTH");
        return false;
    }

    evm_mem_t m_exp = mem_init_immed_variadic(n, args);
    if(!byte_vec_test(m, m_exp)) 
    {
        printf("\n\tMEM ASSERTTION ERROR | BYTES ASSERTTION ERROR");
        return false;
    }
    
    return true;
}

#endif



uint64_t mem_dry_run(evm_mem_p m, evm_word_t w_pos, uint64_t size)
{
    if(!size) return 0;

    word_add_uint64(&w_pos, 0, size);
    word_add_uint64(&w_pos, 0, 0x1f);
    if(!word_is_uint64(&w_pos)) return UINT64_MAX;

    uint64_t max = w_pos.v[0];
    uint64_t m_size_aft = (max > m->size ? max : m->size) >> 5;
    uint64_t m_size_bef = m->size >> 5;
    if(m_size_bef == m_size_aft) return 0;

    uint64_t gas_after = gas_mem(m_size_aft);
    if(gas_after == UINT64_MAX) return UINT64_MAX;

    return gas_after - gas_mem(m_size_bef);;
}

void mem_expand(evm_mem_p m, uint64_t pos)
{
    uint64_t size = (pos + 0x1f) & ~0x1f;
    uint64_t size_prev = m->size;
    if(size <= size_prev) return;

    m->size = size;
    m->v = realloc(m->v, size);
    memset(&m->v[size_prev], 0, size - size_prev);
}



evm_word_t mem_get_word(evm_mem_p m, uint64_t pos)
{
    mem_expand(m, pos+32);
    return bytes_get_word(m, pos);
}

byte_vec_t mem_get_bytes(evm_mem_p m, uint64_t pos, uint64_t size)
{
    if(size == 0) return byte_vec_init_zero();

    mem_expand(m, pos+size);
    byte_t *v = malloc(size);
    assert(v);

    memcpy(v, &m->v[pos], size);
    return (byte_vec_t){size, v};
}



void mem_set_byte(evm_mem_p m, uint64_t pos, byte_t u)
{
    mem_expand(m, pos+1);
    m->v[pos] = u;
}

void mem_set_word(evm_mem_p m, uint64_t pos, evm_word_p w)
{
    mem_expand(m, pos+32);
    for(int i=0; i<32; i++)
        m->v[pos + i] = word_get_byte(w, 31-i);
}

void mem_set_bytes(evm_mem_p m, uint64_t pos, byte_vec_p b)
{
    if(b->size == 0) return;

    mem_expand(m, pos + b->size);
    memcpy(&m->v[pos], b->v, b->size);
}
