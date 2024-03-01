#include "debug.h"
#include "../../utils/assert.h"

#include "../utils/struct.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../utils/debug.h"



void word_display(word_t w)
{
    printf("0x");
    for(uint64_t i=V_MAX-1; i>=0; i--)
        printf(U64PX, w.v[i]);
}

void word_display_immed(word_t w)
{
    printf("\n\t");
    word_display(w);
}



bool word_test(word_t w1, word_t w2)
{
    if(word_eq_bool(&w1, &w2)) return true;

    printf("\n");
    printf("\n\t");word_display(w1);
    printf("\n\t");word_display(w2);
    printf("\n");
    printf("\n\tWORD ASSERTION ERROR");
    return false;
}

#endif



word_t word_init()
{
    return (word_t){{0, 0, 0, 0}};
}

word_t word_init_uint64(uint64_t num)
{
    return (word_t){{num, 0, 0, 0}};
}

word_t word_init_bytes(byte_vec_p b)
{
    uint64_t size = b->size;
    assert(size <= 32);

    word_t w = word_init();
    for(uint64_t i=0; i<size; i++)
        word_set_byte(&w, i, b->v[size-1-i]);
        
    byte_vec_free(b);
    return w;
}



bool word_is_uint64(word_p w)
{
    for(uint64_t i=1; i<V_MAX; i++)
        if(w->v[i])
            return false;

    return true;
}

bool word_eq_bool(word_p w1, word_p w2)
{
    for(uint64_t i=0; i<V_MAX; i++)
        if(w1->v[i] != w2->v[i])
            return false;

    return true;
}

void word_add_uint64(word_p w, uint64_t i, uint64_t v)
{
    if(i >= V_MAX) return;

    uint64_t res = w->v[i] += v;
    if(res < v) word_add_uint64(w, i+1, 1);
}

byte_t word_get_byte(word_p w, uint64_t i)
{
    assert(i<32);
    return ((byte_t*)w->v)[i];
}

void word_set_byte(word_p w, uint64_t i, byte_t u)
{
    assert(i<32);
    ((byte_t*)w->v)[i] = u;
}



word_t word_add(word_p w1, word_p w2)
{
    word_t w = *w1;
    for(uint64_t i=0; i<V_MAX; i++)
        word_add_uint64(&w, i, w2->v[i]);

    return w;
}
