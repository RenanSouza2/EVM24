#include <string.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../stack/list/struct.h"
#include "../../word/debug.h"
#include "../../utils/debug.h"



void test_frame_init()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
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

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
    evm_frame_o_t fo = frame_stop(&f);
    assert(fo.returndata.size == 0);
    assert(fo.returndata.v == NULL);

    assert(mem_empty());
}



void test_frame_pop()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed_setup("0x50", GAS_DEF, 0, 2, W1(1), W1(2));
    assert(frame_pop(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 1, W1(1)));
    frame_free(f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 1, W1(1));
    assert(frame_pop(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 0));
    frame_free(f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 0);
    assert(frame_pop(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x50", 1, 0, 2, W1(1), W1(2));
    assert(frame_pop(&f) == false);
    assert(frame_test_immed(f, IGN, 1, IGN, IGN));
    frame_free(f);

    assert(mem_empty());
}

void test_frame_mload()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed_setup("0x51", GAS_DEF, 0, 1, W1(0x00));
    assert(frame_mload(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, W1(0x00), 1, W1(0x00)));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0x00), 1, W1(0x00));
    assert(frame_mload(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - 3, 1, W1(0x00), 1, W1(0x00)));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x00));
    assert(frame_mload(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - 3, 1, W1(0xff), 1, W1(0xff)));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x10));
    assert(frame_mload(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - 6, 
        2, W1(0xff), W1(0x00),
        1, WORD(0, 0xff, 0, 0)
    ));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 0, 0);
    assert(frame_mload(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", 4, 0, 1, W1(0x00));
    assert(frame_mload(&f) == false);
    assert(frame_test_immed(f, IGN, 4, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", 1, 1, W1(0x00), 1, W1(0x00));
    assert(frame_mload(&f) == false);
    assert(frame_test_immed(f, IGN, 1, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0x00), 1, WORD(0, 0, 1, 0));
    assert(frame_mload(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    assert(mem_empty());
}

void test_frame_mstore()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed_setup("0x51", GAS_DEF, 0, 2, W1(0xff), W1(0x00));
    assert(frame_mstore(&f) == true);
    assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, W1(0xff), 0));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 0, 0);
    assert(frame_mstore(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 0, 2, W1(0), WORD(0, 0, 1, 0));
    assert(frame_mstore(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 0, 1, W1(0));
    assert(frame_mstore(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", 5, 0, 1, W1(0), 2, W1(0xff), W1(0x00));
    assert(frame_mstore(&f) == false);
    assert(frame_test_immed(f, IGN, 5, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", 5, 1, W1(0), 2, W1(0xff), W1(0x00));
    assert(frame_mstore(&f) == true);
    assert(frame_test_immed(f, IGN, 2, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x51", GAS_DEF, 0, 0);
    evm_word_t w = word_init();
    for(int i=0; i<1024; i++)
        assert(stack_push(&f.s, &w));
    assert(frame_mstore(&f) == true);
    assert(frame_test_immed(f, IGN, GAS_DEF - 6, IGN, IGN));
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

        evm_word_t w = word_init();
        memset(w.v, 0xff, i);
        
        evm_frame_t f = frame_init_immed(str, GAS_DEF);
        assert(frame_push(&f) == true);
        assert(frame_test_immed(f, i+1, GAS_DEF - G_very_low, IGN, 1, w));

        frame_free(f);
    }

    evm_frame_t f = frame_init_immed("0x5f", GAS_DEF);
    evm_word_t w = word_init();
    for(int i=0; i<1024; i++)
        assert(stack_push(&f.s, &w) == true);
    assert(frame_push(&f) == false);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed("0x5f", 0);
    assert(frame_push(&f) == false);
    assert(frame_test_immed(f, IGN, 0, IGN, IGN));
    frame_free(f);

    assert(mem_empty());
}



void test_frame()
{
    printf("\n%s\t\t", __func__);

    test_frame_init();

    test_frame_stop();
    test_frame_pop();
    test_frame_mload();
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
