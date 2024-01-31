#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void word_display(word_t w)
{
    printf("0x");
    for(int i=V_MAX-1; i>=0; i--)
        printf("%lx", w.v[i]);
}

bool word_immed(word_t w, u64 v3, u64 v2, u64 v1, u64 v0)
{
    word_t w_exp = WORD(v3, v2, v1, v0);
    return word_eq_bool(&w, &w_exp);
}

#endif

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

word_t word_add(word_t w1, word_t w2)
{
    word_t w = w1;
    for(int i=0; i<V_MAX; i++)
        word_add_immed(&w, i, w2.v[i]);

    return w;
}
