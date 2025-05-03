#include <string.h>

#include "../debug.h"
#include "../../../../testrc.h"

#include "../../mem/debug.h"
#include "../../stack/head/debug.h"
#include "../../gas/header.h"
#include "../../bytes/debug.h"
#include "../../../word/debug.h"
#include "../../../utils/debug.h"



void test_frame_init()
{
    printf("\n\t%s", __func__);

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
    assert_64(f.pc, 0);
    assert_64(f.gas, GAS_DEF);
    assert(byte_vec_test_immed(f.code, "0x"));
    assert(stack_test_immed(f.s, 0));
    assert(mem_immed(f.m, 0));
    frame_free(&f);

    f = frame_init_immed("0xff", GAS_DEF);
    assert_64(f.pc, 0);
    assert_64(f.gas, GAS_DEF);
    assert(byte_vec_test_immed(f.code, "0xff"));
    assert(stack_test_immed(f.s, 0));
    assert(mem_immed(f.m, 0));
    frame_free(&f);

    assert(clu_mem_is_empty());
}

void test_frame_get_jumpdest()
{
    printf("\n\t%s", __func__);

    uint64_vec_t jumpdest = frame_get_jumpdest_immed("0x");
    assert(uint64_vec_test_immed(jumpdest, 0));
    vec_free(&jumpdest);

    jumpdest = frame_get_jumpdest_immed("0x00");
    assert(uint64_vec_test_immed(jumpdest, 0));
    vec_free(&jumpdest);

    jumpdest = frame_get_jumpdest_immed("0x5b");
    assert(uint64_vec_test_immed(jumpdest, 1, 0));
    vec_free(&jumpdest);

    jumpdest = frame_get_jumpdest_immed("0x605b");
    assert(uint64_vec_test_immed(jumpdest, 0));
    vec_free(&jumpdest);

    assert(clu_mem_is_empty());
}



void test_frame_stop()
{
    printf("\n\t%s", __func__);

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
    evm_frame_o_t fo = frame_stop(&f);
    assert(frame_o_test_immed(fo, true, GAS_DEF, "0x"));
    frame_free(&f);

    assert(clu_mem_is_empty());
}



void test_frame_pop()
{
    printf("\n\t%s", __func__);

    evm_frame_t f = frame_init_immed_setup("0x50", GAS_DEF, 0, 2, W1(1), W1(2));
    assert_64(frame_pop(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 1, W1(1)));
    frame_free(&f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 1, W1(1));
    assert_64(frame_pop(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 0));
    frame_free(&f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 0);
    assert_64(frame_pop(&f), 2);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(&f);

    f = frame_init_immed_setup("0x50", 1, 0, 2, W1(1), W1(2));
    assert_64(frame_pop(&f), 1);
    assert(frame_test_immed(f, IGN, 1, IGN, IGN));
    frame_free(&f);

    assert(clu_mem_is_empty());
}

void test_frame_mload()
{
    printf("\n\t%s", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x00));
    assert_64(frame_mload(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - 3, 1, W1(0xff), 1, W1(0xff)));
    frame_free(&f);

    // no items in stack
    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 0);
    assert_64(frame_mload(&f), 1);
    assert(frame_test_immed(f, IGN, GAS_DEF, 1, W1(0xff), 0));
    frame_free(&f);

    // no gas
    f = frame_init_immed_setup("0x51", 0, 1, W1(0xff), 1, W1(0x00));
    assert_64(frame_mload(&f), 2);
    assert(frame_test_immed(f, IGN, 0, 1, W1(0xff), IGN));
    frame_free(&f);

    // expand memory
    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x10));
    assert_64(frame_mload(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - 6,
        2, W1(0xff), W1(0x00),
        1, WORD(0, 0xff, 0, 0)
    ));
    frame_free(&f);

    assert(clu_mem_is_empty());
}

void test_frame_mstore()
{
    printf("\n\t%s", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0xff), W1(0x00));
    assert_64(frame_mstore(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, W1(0xff), 0));
    frame_free(&f);

    // zero elements in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    assert_64(frame_mstore(&f), 1);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // position too high, 64bits
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0), WORD(0, 0, 1, 0));
    assert_64(frame_mstore(&f), 2);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // gas
    f = frame_init_immed_setup("0x52", 5, 0, 2, W1(0xff), W1(0x00));
    assert_64(frame_mstore(&f), 2);
    assert(frame_test_immed(f, IGN, 5, 0, IGN));
    frame_free(&f);

    // one element in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 1, W1(0));
    assert_64(frame_mstore(&f), 3);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // stack full
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    for(int i=0; i<1024; i++)
        assert(!stack_push_immed(&f.s, W1(i)));
    assert_64(frame_mstore(&f), 0);
    assert(frame_test_immed(f, IGN, GAS_DEF - 104, IGN, IGN));
    frame_free(&f);

    assert(clu_mem_is_empty());
}

void test_frame_mstore8()
{
    printf("\n\t%s", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(U64_MAX), W1(0x00));
    assert_64(frame_mstore8(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, WORD(U64_FF, 0, 0, 0), 0));
    frame_free(&f);

    // zero elements in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    assert_64(frame_mstore8(&f), 1);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // position too high, 64bits
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0), WORD(0, 0, 1, 0));
    assert_64(frame_mstore8(&f), 2);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // gas
    f = frame_init_immed_setup("0x52", 5, 0, 2, W1(0xff), W1(0x00));
    assert_64(frame_mstore8(&f), 2);
    assert(frame_test_immed(f, IGN, 5, 0, IGN));
    frame_free(&f);

    // one element in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 1, W1(0));
    assert_64(frame_mstore8(&f), 3);
    assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(&f);

    // stack full
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    for(int i=0; i<1024; i++)
        assert(!stack_push_immed(&f.s, W1(i)));
    assert_64(frame_mstore8(&f), 0);
    assert(frame_test_immed(f, IGN, GAS_DEF - 101, IGN, IGN));
    frame_free(&f);

    assert(clu_mem_is_empty());
}



void test_frame_push()
{
    printf("\n\t%s", __func__);

    evm_frame_t f = frame_init_immed("0x5f", GAS_DEF);
    assert_64(frame_push(&f), 0);
    assert(frame_test_immed(f, 1, GAS_DEF - 2, IGN, 1, W1(0)));
    frame_free(&f);

    for(int i=1; i<=32; i++)
    {
        char str[70];
        memset(str, 0, 70);
        sprintf(str, "0x%x", 0x5f + i);
        memset(&str[4], 'f', 2*i);

        word_t w = W1(0);
        memset(w.arr, 0xff, i);

        f = frame_init_immed(str, GAS_DEF);
        assert_64(frame_push(&f), 0);
        assert(frame_test_immed(f, i+1, GAS_DEF - 3, IGN, 1, w));

        frame_free(&f);
    }

    f = frame_init_immed("0x5f", GAS_DEF);
    for(int i=0; i<1024; i++)
        assert(!stack_push_immed(&f.s, W1(i)));
    assert_64(frame_push(&f), 2);
    assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(&f);

    f = frame_init_immed("0x5f", 0);
    assert_64(frame_push(&f), 1);
    assert(frame_test_immed(f, IGN, 0, IGN, IGN));
    frame_free(&f);

    f = frame_init_immed("0x5f", GAS_DEF);
    for(int i=0; i<1023; i++)
        assert(!stack_push_immed(&f.s, W1(i)));
    assert_64(frame_push(&f), 0);
    assert(frame_test_immed(f, IGN, GAS_DEF - 2, IGN, IGN));
    frame_free(&f);

    assert(clu_mem_is_empty());
}



void test_frame_return()
{
    printf("\n\t%s", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x20), W1(0x00)
    );
    evm_frame_o_t fo = frame_return(&f);
    assert(frame_o_test_immed(fo, true, GAS_DEF, "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
    frame_free(&f);
    frame_o_free(&fo);

    // common case expansion
    f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x20), W1(0x10)
    );
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, true, GAS_DEF - 3, "0x101112131415161718191a1b1c1d1e1f00000000000000000000000000000000"));
    frame_free(&f);
    frame_o_free(&fo);

    // stack 0 elements
    f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        1, W1(0x20)
    );
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, false, GAS_DEF, "0x"));
    frame_free(&f);
    frame_o_free(&fo);

    // stack 1 element
    f = frame_init_immed_setup("0xf3", GAS_DEF, 1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f), 0);
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, false, GAS_DEF, "0x"));
    frame_free(&f);
    frame_o_free(&fo);

    // mem expand too much
    f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x20), W1(U64_MAX)
    );
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, false, GAS_DEF, "0x"));
    frame_free(&f);
    frame_o_free(&fo);

    // return 0
    f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x00), W1(0x00)
    );
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, true, GAS_DEF, "0x"));
    frame_free(&f);
    frame_o_free(&fo);

    // reaturn 0 from far
    f = frame_init_immed_setup("0xf3", GAS_DEF,
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x00), W1(U64_MAX)
    );
    fo = frame_return(&f);
    assert(frame_o_test_immed(fo, true, GAS_DEF, "0x"));
    frame_free(&f);
    frame_o_free(&fo);

    assert(clu_mem_is_empty());
}



void test_frame()
{
    printf("\n%s", __func__);

    test_frame_init();
    test_frame_get_jumpdest();

    test_frame_stop();

    test_frame_pop();
    test_frame_mload();
    test_frame_mstore();
    test_frame_mstore8();

    test_frame_push();

    test_frame_return();

    assert(clu_mem_is_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_frame();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
