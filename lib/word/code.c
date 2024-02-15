#include <assert.h>

#include "debug.h"
#include "../bytes/struct.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../utils/debug.h"



void word_display(evm_word_t w)
{
    printf("0x");
    for(int i=V_MAX-1; i>=0; i--)
        printf(U64PX, w.v[i]);
}

void word_display_immed(evm_word_t w)
{
    printf("\n\t");
    word_display(w);
    printf("");
}



bool word_test(evm_word_t w1, evm_word_t w2)
{
    if(word_eq_bool(&w1, &w2)) return true;

    printf("\n");
    printf("\n\tword1: ");word_display(w1);
    printf("\n\tword2: ");word_display(w2);
    printf("\n\n\tWORD ASSERTION ERROR");
    return false;
}

#endif



evm_word_t word_init()
{
    return (evm_word_t){{0, 0, 0, 0}};
}

evm_word_t word_init_uint64(uint64_t num)
{
    return (evm_word_t){{num, 0, 0, 0}};
}

evm_word_t word_init_bytes(evm_bytes_p b)
{
    int size = b->size;
    assert(size <= 32);

    evm_word_t w = word_init();
    for(int i=0; i<size; i++)
    {
        uchar_t u = bytes_get_byte(b, size-1-i);
        word_set_byte(&w, i, u);
    }
    bytes_free(b);
    return w;
}

bool word_is_uint64(evm_word_p w)
{
    for(int i=1; i<V_MAX; i++)
        if(w->v[i])
            return false;

    return true;
}



bool word_eq_bool(evm_word_p w1, evm_word_p w2)
{
    for(int i=0; i<V_MAX; i++)
        if(w1->v[i] != w2->v[i])
            return false;

    return true;
}

void word_add_uint64(evm_word_p w, int i, uint64_t v)
{
    if(i >= V_MAX) return;

    uint64_t res = w->v[i] += v;
    if(res < v) word_add_uint64(w, i+1, 1);
}

uchar_t word_get_byte(evm_word_p w, int i)
{
    assert(i<32);
    return ((uchar_t*)w->v)[i];
}

void word_set_byte(evm_word_p w, int i, uchar_t u)
{
    assert(i<32);
    ((uchar_t*)w->v)[i] = u;
}



evm_word_t word_add(evm_word_p w1, evm_word_p w2)
{
    evm_word_t w = *w1;
    for(int i=0; i<V_MAX; i++)
        word_add_uint64(&w, i, w2->v[i]);

    return w;
}
