#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../bytes/debug.h"
#include "../../word/debug.h"
#include "../../utils/debug.h"


void test_mem_dry_run()
{
    printf("\n\t%s\t\t", __func__);
    
    evm_mem_t m = mem_init();
    assert(int_test(mem_dry_run(&m,   0),  3));
    assert(int_test(mem_dry_run(&m,   1),  6));
    assert(int_test(mem_dry_run(&m,  31),  6));
    assert(int_test(mem_dry_run(&m,  32),  6));
    assert(int_test(mem_dry_run(&m,  64),  9));
    assert(int_test(mem_dry_run(&m,  96), 12));
    assert(int_test(mem_dry_run(&m, 704), 69));
    assert(int_test(mem_dry_run(&m, 705), 73));

    m.size = 64;
    assert(int_test(mem_dry_run(&m,   0),  3));
    assert(int_test(mem_dry_run(&m,   1),  3));
    assert(int_test(mem_dry_run(&m,  31),  3));
    assert(int_test(mem_dry_run(&m,  32),  3));
    assert(int_test(mem_dry_run(&m,  64),  3));
    assert(int_test(mem_dry_run(&m,  96),  6));
    assert(int_test(mem_dry_run(&m, 704), 63));
    assert(int_test(mem_dry_run(&m, 705), 67));

    m.size = 736;
    assert(int_test(mem_dry_run(&m,    0),  3));
    assert(int_test(mem_dry_run(&m,  736),  3));
    assert(int_test(mem_dry_run(&m,  768),  6));
    assert(int_test(mem_dry_run(&m, 1024), 31));

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
    assert(word_test_immed(w, 0, 0, 0, 0));
    
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_test_immed(w, 0, 0, 0, 0));
    
    bytes_set_byte(&m, 31, 0xff);
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_test_immed(w, 0, 0, 0, 0xff));
    
    w = mem_get_word(&m, 31);
    assert(m.size == 64);
    assert(w.v);
    assert(word_test_immed(w, U64_FF, 0, 0, 0));
    
    w = mem_get_word(&m, 32);
    assert(m.size == 64);
    assert(w.v);
    assert(word_test_immed(w, 0, 0, 0, 0));
    mem_free(m);
    
    assert(mem_empty());
}

void test_mem_set_word()
{
    printf("\n\t%s\t\t", __func__);

    evm_mem_t m = mem_init();
    evm_word_t w = WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f);
    mem_set_word(&m, 0, &w);
    assert(mem_test_immed(m, "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));

    mem_set_word(&m, 16, &w);
    assert(mem_test_immed(m, "0x000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f00000000000000000000000000000000"));
    mem_free(m);

    assert(mem_empty());
}

void test_mem_set_byte()
{
    printf("\n\t%s\t\t", __func__);

    evm_mem_t m = mem_init();
    mem_set_byte(&m, 0, 0xff);
    assert(mem_test_immed(m, "0xff00000000000000000000000000000000000000000000000000000000000000"));

    mem_set_byte(&m, 2, 0xee);
    assert(mem_test_immed(m, "0xff00ee0000000000000000000000000000000000000000000000000000000000"));

    mem_set_byte(&m, 31, 0xdd);
    assert(mem_test_immed(m, "0xff00ee00000000000000000000000000000000000000000000000000000000dd"));

    mem_set_byte(&m, 32, 0xcc);
    assert(mem_test_immed(m, "0xff00ee00000000000000000000000000000000000000000000000000000000ddcc00000000000000000000000000000000000000000000000000000000000000"));
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
