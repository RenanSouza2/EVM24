#include <stdio.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

#include "../../bytes/debug.h"
#include "../../utils/debug.h"



void test_word_size()
{
    printf("\n\t%s", __func__);
    
    assert(sizeof(evm_word_t) == 32);

    assert(mem_empty());
}

void test_word_init()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w = word_init();
    assert(word_test(w, W1(0)));

    assert(mem_empty());
}

void test_word_init_uint_64()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w = word_init_uint64(0);
    assert(word_test(w, W1(0)));
    
    w = word_init_uint64(1);
    assert(word_test(w, W1(1)));
    
    w = word_init_uint64(0xff);
    assert(word_test(w, W1(0xff)));
    
    w = word_init_uint64(U64_FF);
    assert(word_test(w, W1(U64_FF)));
    
    w = word_init_uint64(U64_MAX);
    assert(word_test(w, W1(U64_MAX)));

    assert(mem_empty());
}

void test_word_init_bytes()
{
    printf("\n\t%s", __func__);
    
    evm_bytes_t b = bytes_init_immed("0x");
    evm_word_t w = word_init_bytes(&b);
    assert(word_test(w, W1(0)));

    b = bytes_init_immed("0xff");
    w = word_init_bytes(&b);
    assert(word_test(w, W1(0xff)));

    b = bytes_init_immed("0xffee");
    w = word_init_bytes(&b);
    assert(word_test(w, W1(0xffee)));

    b = bytes_init_immed("0xff0000000000000000");
    w = word_init_bytes(&b);
    assert(word_test(w, WORD(0, 0, 0xff, 0)));

    assert(mem_empty());
}



void test_word_is_uint_64()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w = word_init();
    assert(word_is_uint64(&w) == true);

    w = W1(U64_MAX);
    assert(word_is_uint64(&w) == true);

    w = WORD(0, 0, 1, 0);
    assert(word_is_uint64(&w) == false);

    w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    assert(word_is_uint64(&w) == false);

    assert(mem_empty());
}

void test_word_eq_bool()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w1 = WORD(4, 3, 2, 1);
    evm_word_t w2 = WORD(4, 3, 2, 1);
    assert(word_eq_bool(&w1, &w2) == true);
    
    w2 = WORD(4, 3, 2, 0);
    assert(word_eq_bool(&w1, &w2) == false);
    
    w2 = WORD(4, 3, 0, 1);
    assert(word_eq_bool(&w1, &w2) == false);
    
    w2 = WORD(4, 0, 2, 1);
    assert(word_eq_bool(&w1, &w2) == false);
    
    w2 = WORD(0, 3, 2, 1);
    assert(word_eq_bool(&w1, &w2) == false);

    assert(mem_empty());
}

void test_word_add_uint64()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w = WORD(4, 3, 2, 1);
    word_add_uint64(&w, 0, 1);
    assert(word_test(w, WORD(4, 3, 2, 2)));

    w = WORD(4, 3, 2, 1);
    word_add_uint64(&w, 1, 1);
    assert(word_test(w, WORD(4, 3, 3, 1)));
    
    w = WORD(4, 3, 2, 1);
    word_add_uint64(&w, 2, 1);
    assert(word_test(w, WORD(4, 4, 2, 1)));
    
    w = WORD(4, 3, 2, 1);
    word_add_uint64(&w, 3, 1);
    assert(word_test(w, WORD(5, 3, 2, 1)));
    
    w = WORD(4, 3, 2, 1);
    word_add_uint64(&w, 4, 1);
    assert(word_test(w, WORD(4, 3, 2, 1)));

    w = W1(0xffffffffff);
    word_add_uint64(&w, 0, 0x1f);
    assert(word_test(w, W1(0x1000000001e)));

    w = W1(U64_MAX);
    word_add_uint64(&w, 0, 1);
    assert(word_test(w, WORD(0, 0, 1, 0)));

    w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    word_add_uint64(&w, 0, 1);
    assert(word_test(w, W1(0)));

    assert(mem_empty());
}

void test_word_set_bytes()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w = W1(0);
    word_set_byte(&w, 0, 0xff);
    assert(word_test(w, W1(0xff)));

    w = W1(0);
    word_set_byte(&w, 1, 0xff);
    assert(word_test(w, W1(0xff00)));

    w = W1(0);
    word_set_byte(&w, 7, 0xff);
    assert(word_test(w, W1(U64_FF)));

    w = W1(0);
    word_set_byte(&w, 8, 0xff);
    assert(word_test(w, WORD(0, 0, 0xff, 0)));

    w = W1(0);
    word_set_byte(&w, 31, 0xff);
    assert(word_test(w, WORD(U64_FF, 0, 0, 0)));

    assert(mem_empty());
}



void test_word_add()
{
    printf("\n\t%s", __func__);
    
    evm_word_t w1 = WORD(4, 3, 2, 1);
    evm_word_t w2 = WORD(1, 2, 3, 4);
    evm_word_t w = word_add(&w1, &w2);
    assert(word_test(w, WORD(5, 5, 5, 5)));
    
    w1 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w2 = WORD(0, 0, 1, 0);
    w = word_add(&w1, &w2);
    assert(word_test(w, W1(U64_MAX)));
    
    w1 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w2 = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    w = word_add(&w1, &w2);
    assert(word_test(w, WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX - 1)));

    assert(mem_empty());
}



void test_word()
{
    printf("\n%s", __func__);

    test_word_size();
    test_word_init();
    test_word_init_uint_64();
    test_word_init_bytes();

    test_word_is_uint_64();
    test_word_eq_bool();
    test_word_add_uint64();
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
