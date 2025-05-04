#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../../mods/clu/header.h"
#include "../../../mods/macros/assert.h"

#include "../gas/header.h"
#include "../bytes/header.h"
#include "../../utils/header.h"



#ifdef DEBUG

#include "../../utils/debug.h"
#include "../../word/debug.h"

evm_mem_t mem_init_variadic(uint64_t n, va_list *arg)
{
    evm_mem_t m = byte_vec_init(n << 5);
    for(uint64_t i = 0; i < n; i++)
    {
        word_t w = va_arg(*arg, word_t);
        for(int j = 0; j < 32; j++)
            m.arr[(i << 5) + j] = word_get_byte(&w, 31 - j);
    }
    return m;
}
evm_mem_t mem_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return mem_init_variadic(n, &args);
}



void mem_display(evm_mem_t m)
{
    printf("\nsize: " U64P() "", m.size);
    uint64_t max = m.size >> 5;
    for(uint64_t i=0; i<max; i++)
    {
        printf("\n0x");
        for(uint64_t j=0; j<32; j++)
            printf("%02x", m.arr[(i << 5) + j]);
    }
}



bool mem_test_inner(evm_mem_t m_1, evm_mem_t m_2)
{
    if(!uint64_test(m_1.size, m_2.size))
    {
        printf("\n\tMEM ASSERTTION ERROR | LENGTH");
        return false;
    }

    if(!byte_vec_test(m_1, m_2))
    {
        printf("\n\tMEM ASSERTTION ERROR | BYTES ASSERTTION ERROR");
        return false;
    }

    return true;
}

bool mem_test(evm_mem_t m_1, evm_mem_t m_2)
{
    if(!mem_test_inner(m_1, m_2))
    {
        mem_display(m_1);
        mem_display(m_2);
        return false;
    }

    return true;
}

bool mem_immed(evm_mem_t m, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    evm_mem_t m_2 = mem_init_variadic(n, &args);
    return mem_test(m, m_2);
}

#endif



uint64_t mem_dry_run(evm_mem_p m, word_t w_pos, uint64_t size)
{
    if(!size)
        return 0;

    word_add_uint64(&w_pos, 0, size + 0x1f);
    if(!word_is_uint64(&w_pos))
        return UINT64_MAX;

    uint64_t max = w_pos.arr[0];
    uint64_t m_size_aft = (max > m->size ? max : m->size) >> 5;
    uint64_t m_size_bef = m->size >> 5;
    if(m_size_bef == m_size_aft)
        return 0;

    uint64_t gas_after = gas_mem(m_size_aft);
    if(gas_after == UINT64_MAX)
        return UINT64_MAX;

    return gas_after - gas_mem(m_size_bef);
}

void mem_expand(evm_mem_p m, uint64_t pos)
{
    uint64_t size = (pos + 0x1f) & ~0x1f;
    uint64_t size_prev = m->size;
    if(size <= size_prev)
        return;
    
    m->size = size;
    m->arr = realloc(m->arr, size);
    assert(m->arr != NULL);
    memset(&m->arr[size_prev], 0, size - size_prev);
}

word_t mem_get_word(evm_mem_p m, uint64_t pos)
{
    mem_expand(m, pos + 32);
    return bytes_get_word(m, pos);
}

byte_vec_t mem_get_bytes(evm_mem_p m, uint64_t pos, uint64_t size)
{
    if(size == 0)
        return byte_vec_init_zero();

    mem_expand(m, pos + size);
    byte_t *arr = malloc(size);
    assert(arr);

    memcpy(arr, &m->arr[pos], size);
    return (byte_vec_t)
    {
        .size = size,
        .arr = arr
    };
}

void mem_set_byte(evm_mem_p m, uint64_t pos, byte_t u)
{
    mem_expand(m, pos + 1);
    m->arr[pos] = u;
}

void mem_set_word(evm_mem_p m, uint64_t pos, word_p w)
{
    mem_expand(m, pos + 32);
    for(int i = 0; i < 32; i++)
        m->arr[pos + i] = word_get_byte(w, 31 - i);
}

void mem_set_bytes(evm_mem_p m, uint64_t pos, byte_vec_p b)
{
    if(b->size == 0)
        return;

    mem_expand(m, pos + b->size);
    memcpy(&m->arr[pos], b->arr, b->size);
    vec_free(b);
}
