#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../bytes/debug.h"
#include "../../word/debug.h"
#include "../../utils/debug.h"



void test_mem_dry_run()
{
    printf("\n\t%s\t\t", __func__);
    
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

    assert(mem_empty()); 
}

void test_mem_expand()
{
    printf("\n\t%s\t\t", __func__);
    
    for(int i=0; i<4; i++)
    {
        evm_mem_t m = mem_init();
        mem_expand(&m, 32 * i);
        assert(m.size == 32 * i);
        mem_free(m);

        for(int j=1; j<32; j++)
        {
            evm_mem_t m = mem_init();
            mem_expand(&m, 32 * i + j);
            assert(m.size == 32 * (i + 1));
            mem_free(m);
        }
    }

    assert(mem_empty());
}



void test_mem_get_word()
{
    printf("\n\t%s\t\t", __func__);

    evm_mem_t m = mem_init();
    evm_word_t w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    
    bytes_set_byte(&m, 31, 0xff);
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_test(w, WORD(0, 0, 0, 0xff)));
    
    w = mem_get_word(&m, 31);
    assert(m.size == 64);
    assert(w.v);
    assert(word_test(w, WORD(U64_FF, 0, 0, 0)));
    
    w = mem_get_word(&m, 32);
    assert(m.size == 64);
    assert(w.v);
    assert(word_test(w, WORD(0, 0, 0, 0)));
    mem_free(m);
    
    assert(mem_empty());
}

void test_mem_set_word()
{
    printf("\n\t%s\t\t", __func__);

    evm_mem_t m = mem_init();
    evm_word_t w = WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f);
    mem_set_word(&m, 0, &w);
    assert(mem_test_immed(m, 1, w));

    mem_set_word(&m, 16, &w);
    assert(mem_test_immed(m, 2,
        WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x0001020304050607, 0x08090a0b0c0d0e0f),
        WORD(0x1011121314151617, 0x18191a1b1c1d1e1f, 0, 0)
    ));
    mem_free(m);

    assert(mem_empty());
}

void test_mem_set_byte()
{
    printf("\n\t%s\t\t", __func__);

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
    mem_free(m);

    assert(mem_empty());
}



void test_mem()
{
    printf("\n%s\t\t", __func__);

    test_mem_dry_run();
    test_mem_expand();

    test_mem_get_word();
    test_mem_set_word();
    test_mem_set_byte();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
