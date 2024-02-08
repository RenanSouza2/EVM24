#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../bytes/debug.h"
#include "../../utils/debug.h"



void test_word_size()
{
    printf("\n\t%s\t\t", __func__);
    
    assert(sizeof(word_t) == 32);

    assert(mem_empty());
}

void test_word_from_zero()
{
    printf("\n\t%s\t\t", __func__);
    
    word_t w = word_from_zero();
    assert(word_immed(w, 0, 0, 0, 0));

    assert(mem_empty());
}

void test_word_from_bytes()
{
    printf("\n\t%s\t\t", __func__);
    
    bytes_t b = bytes_init_immed("0x");
    word_t w = word_from_bytes(&b);
    assert(word_immed(w, 0, 0, 0, 0));

    b = bytes_init_immed("0xff");
    w = word_from_bytes(&b);
    assert(word_immed(w, 0, 0, 0, 0xff));

    b = bytes_init_immed("0xffee");
    w = word_from_bytes(&b);
    assert(word_immed(w, 0, 0, 0, 0xffee));

    b = bytes_init_immed("0xff0000000000000000");
    w = word_from_bytes(&b);
    assert(word_immed(w, 0, 0, 0xff, 0));

    assert(mem_empty());
}



void test_word_eq_bool()
{
    printf("\n\t%s\t\t", __func__);
    
    word_t w = WORD(4, 3, 2, 1);
    assert(word_immed(w, 4, 3, 2, 1) == true);
    assert(word_immed(w, 4, 3, 2, 0) == false);
    assert(word_immed(w, 4, 3, 0, 1) == false);
    assert(word_immed(w, 4, 0, 2, 1) == false);
    assert(word_immed(w, 0, 3, 2, 1) == false);

    assert(mem_empty());
}

void test_word_add_immed()
{
    printf("\n\t%s\t\t", __func__);
    
    word_t w = WORD(4, 3, 2, 1);
    word_add_immed(&w, 0, 1);
    assert(word_immed(w, 4, 3, 2, 2) == true);

    w = WORD(4, 3, 2, 1);
    word_add_immed(&w, 1, 1);
    assert(word_immed(w, 4, 3, 3, 1) == true);
    
    w = WORD(4, 3, 2, 1);
    word_add_immed(&w, 2, 1);
    assert(word_immed(w, 4, 4, 2, 1) == true);
    
    w = WORD(4, 3, 2, 1);
    word_add_immed(&w, 3, 1);
    assert(word_immed(w, 5, 3, 2, 1) == true);
    
    w = WORD(4, 3, 2, 1);
    word_add_immed(&w, 4, 1);
    assert(word_immed(w, 4, 3, 2, 1) == true);

    w = WORD(0, 0, 0, U64_MAX);
    word_add_immed(&w, 0, 1);
    assert(word_immed(w, 0, 0, 1, 0) == true);

    w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    word_add_immed(&w, 0, 1);
    assert(word_immed(w, 0, 0, 0, 0) == true);

    assert(mem_empty());
}

void test_word_set_bytes()
{
    printf("\n\t%s\t\t", __func__);
    
    word_t w = WORD(0, 0, 0, 0);
    word_set_byte(&w, 0, 0xff);
    assert(word_immed(w, 0, 0, 0, 0xff));

    w = WORD(0, 0, 0, 0);
    word_set_byte(&w, 1, 0xff);
    assert(word_immed(w, 0, 0, 0, 0xff00));

    w = WORD(0, 0, 0, 0);
    word_set_byte(&w, 7, 0xff);
    assert(word_immed(w, 0, 0, 0, U64_FF));

    w = WORD(0, 0, 0, 0);
    word_set_byte(&w, 8, 0xff);
    assert(word_immed(w, 0, 0, 0xff, 0));

    w = WORD(0, 0, 0, 0);
    word_set_byte(&w, 31, 0xff);
    assert(word_immed(w, U64_FF, 0, 0, 0));

    assert(mem_empty());
}



void test_word_add()
{
    printf("\n\t%s\t\t", __func__);
    
    word_t w1 = WORD(4, 3, 2, 1);
    word_t w2 = WORD(1, 2, 3, 4);
    word_t w = word_add(&w1, &w2);
    assert(word_immed(w, 5, 5, 5, 5) == true);
    
    w1 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w2 = WORD(0, 0, 1, 0);
    w = word_add(&w1, &w2);
    assert(word_immed(w, 0, 0, 0, U64_MAX) == true);
    
    w1 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w2 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w = word_add(&w1, &w2);
    assert(word_immed(w, U64_MAX, U64_MAX, U64_MAX, U64_MAX - 1) == true);

    assert(mem_empty());
}



void test_word()
{
    printf("\n%s\t\t", __func__);

    test_word_size();
    test_word_from_zero();
    test_word_from_bytes();

    test_word_eq_bool();
    test_word_add_immed();
    test_word_set_bytes();

    test_word_add();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_word();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
