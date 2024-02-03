#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../word/debug.h"



void test_bytes_cton()
{
    printf("\n\t%s\t\t", __func__);

    char ascii[] = {
        '0', '1', '2', '3', 
        '4', '5', '6', '7', 
        '8', '9', 'a', 'b', 
        'c', 'd', 'e', 'f'
    };

    for(int i=0; i<16; i++)
        assert(cton(ascii[i]) == i);

    char ASCII[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i=0; i<6; i++)
        assert(cton(ASCII[i]) == i + 10);

    assert(mem_empty());
}

void test_bytes_init_immed()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    assert(b.v == NULL);
    assert(b.size == 0);

    b = bytes_init_immed("0x00");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 0);
    bytes_free(b);

    b = bytes_init_immed("0x01");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 1);
    bytes_free(b);

    b = bytes_init_immed("0x0f");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 15);
    bytes_free(b);

    b = bytes_init_immed("0x10");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 16);
    bytes_free(b);

    b = bytes_init_immed("0x80");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 128);
    bytes_free(b);

    b = bytes_init_immed("0xf0");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 240);
    bytes_free(b);

    b = bytes_init_immed("0xff");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 255);
    bytes_free(b);

    b = bytes_init_immed("0x0000");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 0);
    assert(b.v[1] == 0);
    bytes_free(b);

    b = bytes_init_immed("0x1234");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 18);
    assert(b.v[1] == 52);
    bytes_free(b);

    assert(mem_empty());
}



void test_bytes_expand()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    bytes_expand(&b, 0);
    assert(b.size == 0);
    assert(b.v == NULL);

    bytes_expand(&b, 0);
    assert(b.size == 0);
    assert(b.v == NULL);
    bytes_free(b);

    b = bytes_init_immed("0xff");
    bytes_expand(&b, 2);
    assert(b.size == 2);
    assert(b.v[0] == 0xff);
    assert(b.v[1] == 0x00);
    bytes_free(b);

    assert(mem_empty());
}

void test_bytes_get_byte()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    for(int i=0; i<32; i++)
        assert(bytes_get_byte(&b, i) == i);
    assert(bytes_get_byte(&b, 64) == 0);
    bytes_free(b);

    b = bytes_init_immed("0x");
    assert(bytes_get_byte(&b, 0) == 0);

    assert(mem_empty());
}

void test_bytes_set_byte()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    bytes_set_byte(&b, 0, 0xff);
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 0xff);
    
    bytes_set_byte(&b, 2, 0xff);
    assert(b.size == 3);
    assert(b.v);
    assert(b.v[1] == 0x00);
    assert(b.v[2] == 0xff);

    bytes_set_byte(&b, 0, 0xfe);
    assert(b.size == 3);
    assert(b.v);
    assert(b.v[0] == 0xfe);
    bytes_free(b);

    assert(mem_empty());
}

void test_bytes_get_word()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    word_t w = bytes_get_word(&b, 0);
    assert(word_immed(w, 0, 0, 0, 0));
    bytes_free(b);

    b = bytes_init_immed("0x");
    bytes_set_byte(&b, 0, 0xff);
    w = bytes_get_word(&b, 0);
    assert(word_immed(w, U64_FF, 0, 0, 0));

    w = bytes_get_word(&b, 1);
    assert(word_immed(w, 0, 0, 0, 0));
    bytes_free(b);

    b = bytes_init_immed("0x");
    bytes_set_byte(&b, 31, 0xff);
    w = bytes_get_word(&b, 0);
    assert(word_immed(w, 0, 0, 0, 0xff));

    w = bytes_get_word(&b, 1);
    assert(word_immed(w, 0, 0, 0, 0xff00));
    bytes_free(b);

    assert(mem_empty());
}

void test_bytes_set_word()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    word_t w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    bytes_set_word(&b, 0, &w);
    assert(b.size == 32);
    for(int i=0; i<32; i++)
        assert(bytes_get_byte(&b, i) == 0xff);
    assert(bytes_get_byte(&b, 32) == 0x00);

    for(int i=0; i<32; i++)
        word_set_byte(&w, i, 0xee);
    
    bytes_set_word(&b, 1, &w);
    assert(bytes_get_byte(&b, 0) == 0xff);
    for(int i=1; i<33; i++)
        assert(bytes_get_byte(&b, i) == 0xee);
    assert(bytes_get_byte(&b, 33) == 0x00);

    assert(mem_empty());
}





void test_bytes()
{
    printf("\n%s\t\t", __func__);

    test_bytes_cton();
    test_bytes_init_immed();

    test_bytes_expand();
    test_bytes_get_byte();
    test_bytes_set_byte();
    test_bytes_get_word();
    test_bytes_set_word();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
