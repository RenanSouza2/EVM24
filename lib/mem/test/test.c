#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

#include "../../bytes/debug.h"
#include "../../word/debug.h"
#include "../../utils/debug.h"



void test_mem_dry_run()
{
    printf("\n\t%s", __func__);
    
    evm_mem_t m = mem_init();
    assert_64(mem_dry_run(&m, W1(  1), 0),  0);
    assert_64(mem_dry_run(&m, W1(  0), 1),  3);
    assert_64(mem_dry_run(&m, W1( 30), 1),  3);
    assert_64(mem_dry_run(&m, W1( 31), 1),  3);
    assert_64(mem_dry_run(&m, W1( 63), 1),  6);
    assert_64(mem_dry_run(&m, W1( 95), 1),  9);
    assert_64(mem_dry_run(&m, W1(703), 1), 66);
    assert_64(mem_dry_run(&m, W1(704), 1), 70);

    m.size = 64;
    assert_64(mem_dry_run(&m, W1(  0), 1),  0);
    assert_64(mem_dry_run(&m, W1( 30), 1),  0);
    assert_64(mem_dry_run(&m, W1( 31), 1),  0);
    assert_64(mem_dry_run(&m, W1( 63), 1),  0);
    assert_64(mem_dry_run(&m, W1( 95), 1),  3);
    assert_64(mem_dry_run(&m, W1(703), 1), 60);
    assert_64(mem_dry_run(&m, W1(704), 1), 64);

    m.size = 736;
    assert_64(mem_dry_run(&m, W1(   0), 1),  0);
    assert_64(mem_dry_run(&m, W1( 735), 1),  0);
    assert_64(mem_dry_run(&m, W1( 767), 1),  3);
    assert_64(mem_dry_run(&m, W1(1023), 1), 28);

    assert_64(mem_dry_run(&m, W1(U64_MAX - 1), 1), U64_MAX);
    assert_64(mem_dry_run(&m, W1(U64_MAX), 1), U64_MAX);
    assert_64(mem_dry_run(&m, W1(0xfffffffffffe), 1), U64_MAX);
    assert_64(mem_dry_run(&m,   WORD(0, 0, 1, 0), 1), U64_MAX);

    m.size = U64_MAX;
    assert_64(mem_dry_run(&m, W1(U64_MAX), 1), U64_MAX);

    assert(clu_mem_empty()); 
}

void test_mem_expand()
{
    printf("\n\t%s", __func__);
    
    for(int i=0; i<4; i++)
    {
        evm_mem_t m = mem_init();
        mem_expand(&m, 32 * i);
        assert(m.size == 32 * i);
        mem_free(&m);

        for(int j=1; j<32; j++)
        {
            evm_mem_t m = mem_init();
            mem_expand(&m, 32 * i + j);
            assert(m.size == 32 * (i + 1));
            mem_free(&m);
        }
    }

    assert(clu_mem_empty());
}



void test_mem_get_word()
{
    printf("\n\t%s", __func__);

    evm_mem_t m = mem_init();
    evm_word_t w = mem_get_word(&m, 0);
    assert(mem_test_immed(m, 1, W1(0)));
    assert(word_test(w, W1(0)));
    mem_free(&m);
    
    m = mem_init_immed(1, W1(0xff));
    w = mem_get_word(&m, 0);
    assert(mem_test_immed(m, 1, W1(0xff)));
    assert(word_test(w, W1(0xff)));
    
    w = mem_get_word(&m, 31);
    assert(mem_test_immed(m, 2, W1(0xff), W1(0)));
    assert(word_test(w, WORD(U64_FF, 0, 0, 0)));
    
    w = mem_get_word(&m, 32);
    assert(mem_test_immed(m, 2, W1(0xff), W1(0)));
    assert(word_test(w, W1(0)));
    mem_free(&m);
    
    assert(clu_mem_empty());
}

void test_mem_get_bytes()
{
    printf("\n\t%s", __func__);

    evm_mem_t m = mem_init_immed(1, W1(0x1234));
    evm_bytes_t b = mem_get_bytes(&m, 0x1e, 2);
    assert(bytes_test_immed(b, "0x1234"));
    assert(mem_test_immed(m, 1, W1(0x1234)));
    bytes_free(&b);

    b = mem_get_bytes(&m, 0x40, 0);
    assert(bytes_test_immed(b, "0x"));
    assert(mem_test_immed(m, 1, W1(0x1234)));
    bytes_free(&b);

    b = mem_get_bytes(&m, 0x1e, 3);
    assert(bytes_test_immed(b, "0x123400"));
    assert(mem_test_immed(m, 2, W1(0x1234), W1(0)));
    bytes_free(&b);
    mem_free(&m);
    
    assert(clu_mem_empty());
}



void test_mem_set_byte()
{
    printf("\n\t%s", __func__);

    evm_mem_t m = mem_init();
    mem_set_byte(&m, 0, 0xff);
    assert(mem_test_immed(m, 1, WORD(U64_FF, 0, 0, 0)));

    mem_set_byte(&m, 2, 0xee);
    assert(mem_test_immed(m, 1, WORD(0xff00ee0000000000, 0, 0, 0)));

    mem_set_byte(&m, 31, 0xdd);
    assert(mem_test_immed(m, 1, WORD(0xff00ee0000000000, 0, 0, 0xdd)));

    mem_set_byte(&m, 32, 0xcc);
    assert(mem_test_immed(m, 2, 
        WORD(0xff00ee0000000000, 0, 0, 0xdd),
        WORD(0xcc00000000000000, 0, 0, 0)
    ));
    mem_free(&m);

    assert(clu_mem_empty());
}

void test_mem_set_word()
{
    printf("\n\t%s", __func__);

    evm_mem_t m = mem_init();
    evm_word_t w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    mem_set_word(&m, 0, &w);
    assert(mem_test_immed(m, 1, WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX)));

    w = WORD(0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee);
    mem_set_word(&m, 1, &w);
    assert(mem_test_immed(m, 2,
        WORD(0xffeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee),
        WORD(0xee00000000000000, 0, 0, 0)
    ));
    mem_free(&m);

    assert(clu_mem_empty());
}

void test_mem_set_bytes()
{
    printf("\n\t%s", __func__);

    evm_mem_t m = mem_init();
    evm_bytes_t b = bytes_init();
    mem_set_bytes(&m, 1024, &b);
    assert(mem_test_immed(m, 0));
    
    b = bytes_init_immed("0x1234");
    mem_set_bytes(&m, 0, &b);
    assert(mem_test_immed(m, 1, WORD(0x1234000000000000, 0, 0, 0)));
    bytes_free(&b);
    
    b = bytes_init_immed("0x5678");
    mem_set_bytes(&m, 1, &b);
    assert(mem_test_immed(m, 1, WORD(0x1256780000000000, 0, 0, 0)));
    bytes_free(&b);
    mem_free(&m);

    m = mem_init();
    b = bytes_init_immed("0x1234");
    mem_set_bytes(&m, 0x1f, &b);
    assert(mem_test_immed(m, 2, W1(0x12),  WORD(0x3400000000000000, 0, 0, 0)));
    bytes_free(&b);
    mem_free(&m);

    assert(clu_mem_empty());
}



void test_mem()
{
    printf("\n%s", __func__);

    test_mem_dry_run();
    test_mem_expand();

    test_mem_get_word();
    test_mem_get_bytes();

    test_mem_set_byte();
    test_mem_set_word();
    test_mem_set_bytes();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
