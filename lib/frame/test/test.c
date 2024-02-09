#include <string.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../stack/list/struct.h"
#include "../../word/debug.h"



void test_frame_init()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed("0x", GAS_DEF);
    assert(f.pc == 0);
    assert(f.gas == GAS_DEF);
    assert(f.code.size == 0);
    assert(f.code.v == NULL);
    assert(f.s.count == 0);
    assert(f.s.sl == NULL);
    assert(f.m.size == 0);
    assert(f.m.v == NULL);

    f = frame_init_immed("0xff", GAS_DEF);
    assert(f.pc == 0);
    assert(f.gas == GAS_DEF);
    assert(f.code.size == 1);
    assert(f.code.v != NULL);
    assert(f.code.v[0] == 0xff);
    assert(f.s.count == 0);
    assert(f.s.sl == NULL);
    assert(f.m.size == 0);
    assert(f.m.v == NULL);
    frame_free(f);

    assert(mem_empty());
}



void test_frame_stop()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed("0x", GAS_DEF);
    frame_o_t fo = frame_stop(&f);
    assert(fo.returndata.size == 0);
    assert(fo.returndata.v == NULL);

    assert(mem_empty());
}

void test_frame_pop()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed_setup("0x50", GAS_DEF, "0x", 2, W1(1), W1(2));

    assert(frame_pop(&f) == true);
    assert(frame_immed(f, 1, GAS_DEF - G_base, NULL, 1, W1(1)));

    f.pc = 0;
    assert(frame_pop(&f) == true);
    assert(frame_immed(f, 1, GAS_DEF - 2 * G_base, NULL, 0));

    f.pc = 0;
    assert(frame_pop(&f) == false);
    assert(frame_immed(f, 0, GAS_DEF - 2 * G_base, NULL, 0));
    frame_free(f);

    assert(mem_empty());
}

void test_frame_mstore()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed_setup("0x51", GAS_DEF, "0x", 2, W1(0xff), W1(0x00));
    assert(frame_mstore(&f) == true);
    assert(frame_immed(f, 1, GAS_DEF, "0x00000000000000000000000000000000000000000000000000000000000000ff", 0));
    frame_free(f);

    assert(mem_empty());
}

void test_frame_push()
{
    printf("\n\t%s\t\t", __func__);

    for(int i=0; i<=32; i++)
    {
        char str[70];
        memset(str, 0, 70);
        sprintf(str, "0x%x", 0x5f + i);
        memset(&str[4], 'f', 2*i);

        word_t w = word_from_zero();
        memset(w.v, 0xff, i);
        
        frame_t f = frame_init_immed(str, GAS_DEF);
        assert(frame_push(&f) == true);
        assert(frame_immed(f, i+1, GAS_DEF, "0x", 1, w));

        frame_free(f);
    }

    frame_t f = frame_init_immed("0x5f", GAS_DEF);
    word_t w = word_from_zero();
    for(int i=0; i<1024; i++)
        assert(stack_evm_push(&f.s, &w) == true);
    assert(frame_push(&f) == false);
    frame_free(f);

    assert(mem_empty());
}



void test_frame()
{
    printf("\n%s\t\t", __func__);

    test_frame_init();

    test_frame_stop();
    test_frame_pop();
    test_frame_mstore();
    test_frame_push();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_frame();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
