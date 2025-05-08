#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../utils/header.h"



#ifdef DEBUG

void word_display(word_t w)
{
    printf("0x");
    for(int64_t i=V_MAX-1; i>=0; i--)
        printf(U64PX, w.arr[i]);
}



bool word_test(word_t w1, word_t w2)
{
    if(word_eq(&w1, &w2)) return true;

    printf("\n");
    printf("\n\t");word_display(w1);
    printf("\n\t");word_display(w2);
    printf("\n");
    printf("\n\tWORD ASSERTION ERROR");
    return false;
}

#endif



word_t word_init_bytes(byte_vec_p b)
{
    uint64_t size = b->size;
    assert(size <= 32);

    word_t w = W1(0);
    for(uint64_t i=0; i<size; i++)
    {
        byte_t u = b->arr[size-1-i];
        word_set_byte(&w, 31-i, u);
    }
    vec_free(b);
    return w;
}



byte_t word_get_byte(word_p w, uint64_t index)
{
    assert(index<32);
    return ((byte_t*)w->arr)[31-index];
}

void word_set_byte(word_p w, uint64_t index, byte_t u)
{
    assert(index<32);
    ((byte_t*)(w->arr))[31-index] = u;
}



bool word_eq(word_p w1, word_p w2)
{
    for(uint64_t i=0; i<V_MAX; i++)
        if(w1->arr[i] != w2->arr[i])
            return false;

    return true;
}



bool word_is_uint64(word_p w)
{
    for(uint64_t i=1; i<V_MAX; i++)
        if(w->arr[i])
            return false;

    return true;
}

void word_add_uint64(word_p w, uint64_t i, uint64_t v)
{
    if(i >= V_MAX) return;

    uint64_t res = w->arr[i] += v;
    if(res < v) word_add_uint64(w, i+1, 1);
}

word_t word_add(word_p w1, word_p w2)
{
    word_t w = *w1;
    for(uint64_t i=0; i<V_MAX; i++)
        word_add_uint64(&w, i, w2->arr[i]);

    return w;
}



// TODO test
uint64_t word_get_size(word_p w)
{
    for(uint64_t i=64; i>0; i--)
        if(word_get_byte(w, i-1) != 0x00)
            return i;

    return 0;
}

// TODO test
byte_vec_t byte_vec_init_word(word_p w)
{
    uint64_t size = word_get_size(w);
    byte_vec_t b = byte_vec_init(size);
    for(uint64_t i=0; i<size; i++)
        b.arr[size-1 - i] = word_get_byte(w, i);

    return b;
}
