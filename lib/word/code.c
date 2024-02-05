#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"
#include "../bytes/struct.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void word_display(word_t w)
{
    printf("0x");
    for(int i=V_MAX-1; i>=0; i--)
        printf("%016lx", w.v[i]);
}

void word_display_immed(word_t w)
{
    printf("\n\t");
    word_display(w);
    printf("\t\t");
}

bool word_immed(word_t w, u64 v3, u64 v2, u64 v1, u64 v0)
{
    word_t w_exp = WORD(v3, v2, v1, v0);
    return word_eq_bool(&w, &w_exp);
}

#endif



word_t word_from_zero()
{
    return (word_t){{0, 0, 0, 0}};
}

word_t word_from_bytes(bytes_p b)
{
    int size = b->size;
    assert(size <= 32);

    word_t w = word_from_zero();
    for(int i=0; i<size; i++)
    {
        uchar u = bytes_get_byte(b, size-1-i);
        word_set_byte(&w, i, u);
    }
    bytes_free(b);
    return w;
}



bool word_eq_bool(word_p w1, word_p w2)
{
    for(int i=0; i<V_MAX; i++)
        if(w1->v[i] != w2->v[i])
            return false;

    return true;
}

void word_add_immed(word_p w, int i, u64 v)
{
    if(i >= V_MAX) return;

    int res = w->v[i] += v;
    if(res < v)
        word_add_immed(w, i+1, 1);
}

uchar word_get_byte(word_p w, int i)
{
    assert(i<32);
    return ((uchar*)w->v)[i];
}

void word_set_byte(word_p w, int i, uchar u)
{
    assert(i<32);
    ((uchar*)w->v)[i] = u;
}



word_t word_add(word_p w1, word_p w2)
{
    word_t w = *w1;
    for(int i=0; i<V_MAX; i++)
        word_add_immed(&w, i, w2->v[i]);

    return w;
}
