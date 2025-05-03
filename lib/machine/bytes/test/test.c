#include "../debug.h"
#include "../../../../testrc.h"

#include "../../../word/debug.h"
#include "../../../utils/debug.h"



void test_bytes_get_byte()
{
    printf("\n\t%s", __func__);

    evm_bytes_t b = byte_vec_init_immed("0x");
    assert(bytes_get_byte(&b, 0) == 0);

    b = byte_vec_init_immed("0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    for(int i=0; i<32; i++)
        assert(bytes_get_byte(&b, i) == i);
    assert(bytes_get_byte(&b, 64) == 0);
    vec_free(&b);

    assert(clu_mem_is_empty());
}

void test_bytes_get_word()
{
    printf("\n\t%s", __func__);

    // printf("\n\t\t%s 1", __func__);
    evm_bytes_t b = byte_vec_init_immed("0x");
    word_t w = bytes_get_word(&b, 0);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    vec_free(&b);

    // printf("\n\t\t%s 2", __func__);
    b = byte_vec_init_immed("0xff");
    w = bytes_get_word(&b, 0);
    assert(word_test(w, WORD(U64_FF, 0, 0, 0)));

    // printf("\n\t\t%s 3", __func__);
    w = bytes_get_word(&b, 1);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    vec_free(&b);

    // printf("\n\t\t%s 4", __func__);
    b = byte_vec_init_immed("0x00000000000000000000000000000000000000000000000000000000000000ff");
    w = bytes_get_word(&b, 0);
    assert(word_test(w, WORD(0, 0, 0, 0xff)));

    // printf("\n\t\t%s 5", __func__);
    w = bytes_get_word(&b, 1);
    assert(word_test(w, WORD(0, 0, 0, 0xff00)));

    // printf("\n\t\t%s 6", __func__);
    w = bytes_get_word(&b, 31);
    assert(word_test(w, WORD(U64_FF, 0, 0, 0)));

    // printf("\n\t\t%s 7", __func__);
    w = bytes_get_word(&b, 32);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    vec_free(&b);

    assert(clu_mem_is_empty());
}

void test_bytes_get_bytes()
{
    printf("\n\t%s", __func__);

    evm_bytes_t b0 = byte_vec_init_immed("0x0001020304");
    evm_bytes_t b1 = bytes_get_bytes(&b0, 0, 0);
    assert(byte_vec_test_immed(b1, "0x"));

    b1 = bytes_get_bytes(&b0, 0, 6);
    assert(byte_vec_test_immed(b1, "0x000102030400"));
    vec_free(&b1);

    b1 = bytes_get_bytes(&b0, 2, 2);
    assert(byte_vec_test_immed(b1, "0x0203"));
    vec_free(&b0);
    vec_free(&b1);

    assert(clu_mem_is_empty());
}



void test_bytes()
{
    printf("\n%s", __func__);

    test_bytes_get_byte();
    test_bytes_get_word();
    test_bytes_get_bytes();

    assert(clu_mem_is_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
