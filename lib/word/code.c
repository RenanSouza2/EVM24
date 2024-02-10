#include <stdio.h>
#include <stdlib.h>
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
        uint64_display(w.v[i]);
}

void word_display_immed(evm_word_t w)
{
    printf("\n\t");
    word_display(w);
    printf("\t\t");
}



bool word(evm_word_t w1, evm_word_t w2)
{
    if(word_eq_bool(&w1, &w2))
        return true;

    printf("\n");
    printf("\n\tword1: ");word_display(w1);
    printf("\n\tword2: ");word_display(w2);
    printf("\n\n\tWORD ASSERTION ERROR 1 | WORD DID NOT MATCH");
    return false;
}

bool word_immed(evm_word_t w, uint64_t v3, uint64_t v2, uint64_t v1, uint64_t v0)
{
    evm_word_t w_exp = WORD(v3, v2, v1, v0);
    return word(w, w_exp);
}

#endif



evm_word_t word_from_zero()
{
    return (evm_word_t){{0, 0, 0, 0}};
}

evm_word_t word_from_bytes(evm_bytes_p b)
{
    int size = b->size;
    assert(size <= 32);

    evm_word_t w = word_from_zero();
    for(int i=0; i<size; i++)
    {
        uchar u = bytes_get_byte(b, size-1-i);
        word_set_byte(&w, i, u);
    }
    bytes_free(b);
    return w;
}



bool word_eq_bool(evm_word_p w1, evm_word_p w2)
{
    for(int i=0; i<V_MAX; i++)
        if(w1->v[i] != w2->v[i])
            return false;

    return true;
}

void word_add_immed(evm_word_p w, int i, uint64_t v)
{
    if(i >= V_MAX) return;

    int res = w->v[i] += v;
    if(res < v)
        word_add_immed(w, i+1, 1);
}

uchar word_get_byte(evm_word_p w, int i)
{
    assert(i<32);
    return ((uchar*)w->v)[i];
}

void word_set_byte(evm_word_p w, int i, uchar u)
{
    assert(i<32);
    ((uchar*)w->v)[i] = u;
}



evm_word_t word_add(evm_word_p w1, evm_word_p w2)
{
    evm_word_t w = *w1;
    for(int i=0; i<V_MAX; i++)
        word_add_immed(&w, i, w2->v[i]);

    return w;
}
