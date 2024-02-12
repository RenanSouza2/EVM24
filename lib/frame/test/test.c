#include <string.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../stack/head/debug.h"
#include "../../stack/list/struct.h"
#include "../../word/debug.h"
#include "../../utils/debug.h"



void test_frame_init()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
    _assert(f.pc == 0);
    _assert(f.gas == GAS_DEF);
    _assert(f.code.size == 0);
    _assert(f.code.v == NULL);
    _assert(f.s.count == 0);
    _assert(f.s.sl == NULL);
    _assert(f.m.size == 0);
    _assert(f.m.v == NULL);

    f = frame_init_immed("0xff", GAS_DEF);
    _assert(f.pc == 0);
    _assert(f.gas == GAS_DEF);
    _assert(f.code.size == 1);
    _assert(f.code.v != NULL);
    _assert(f.code.v[0] == 0xff);
    _assert(f.s.count == 0);
    _assert(f.s.sl == NULL);
    _assert(f.m.size == 0);
    _assert(f.m.v == NULL);
    frame_free(f);

    _assert(mem_empty());
}



void test_frame_stop()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed("0x", GAS_DEF);
    evm_frame_o_t fo = frame_stop(&f);
    _assert(fo.returndata.size == 0);
    _assert(fo.returndata.v == NULL);

    _assert(mem_empty());
}



void test_frame_pop()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed_setup("0x50", GAS_DEF, 0, 2, W1(1), W1(2));
    assert_int(frame_pop(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 1, W1(1)));
    frame_free(f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 1, W1(1));
    assert_int(frame_pop(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - G_base, IGN, 0));
    frame_free(f);

    f = frame_init_immed_setup("0x50", GAS_DEF, 0, 0);
    assert_int(frame_pop(&f), 2);
    _assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed_setup("0x50", 1, 0, 2, W1(1), W1(2));
    assert_int(frame_pop(&f), 1);
    _assert(frame_test_immed(f, IGN, 1, IGN, IGN));
    frame_free(f);

    _assert(mem_empty());
}

void test_frame_mload()
{
    printf("\n\t%s\t\t", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x00));
    assert_int(frame_mload(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - 3, 1, W1(0xff), 1, W1(0xff)));
    frame_free(f);

    // no items in stack
    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 0);
    assert_int(frame_mload(&f), 1);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 1, W1(0xff), 0));
    frame_free(f);

    // no gas
    f = frame_init_immed_setup("0x51", 0, 1, W1(0xff), 1, W1(0x00));
    assert_int(frame_mload(&f), 2);
    _assert(frame_test_immed(f, IGN, 0, 1, W1(0xff), IGN));
    frame_free(f);

    // expand memory
    f = frame_init_immed_setup("0x51", GAS_DEF, 1, W1(0xff), 1, W1(0x10));
    assert_int(frame_mload(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - 6, 
        2, W1(0xff), W1(0x00),
        1, WORD(0, 0xff, 0, 0)
    ));
    frame_free(f);

    _assert(mem_empty());
}

void test_frame_mstore()
{
    printf("\n\t%s\t\t", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0xff), W1(0x00));
    assert_int(frame_mstore(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, W1(0xff), 0));
    frame_free(f);

    // zero elements in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    assert_int(frame_mstore(&f), 1);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // position too high, 64bits
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0), WORD(0, 0, 1, 0));
    assert_int(frame_mstore(&f), 2);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // gas
    f = frame_init_immed_setup("0x52", 5, 0, 2, W1(0xff), W1(0x00));
    assert_int(frame_mstore(&f), 2);
    _assert(frame_test_immed(f, IGN, 5, 0, IGN));
    frame_free(f);

    // one element in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 1, W1(0));
    assert_int(frame_mstore(&f), 3);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // stack full
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    for(int i=0; i<1024; i++)
        _assert(!stack_push_immed(&f.s, W1(i)));
    assert_int(frame_mstore(&f), 0);
    _assert(frame_test_immed(f, IGN, GAS_DEF - 104, IGN, IGN));
    frame_free(f);

    _assert(mem_empty());
}

void test_frame_mstore8()
{
    printf("\n\t%s\t\t", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(U64_MAX), W1(0x00));
    assert_int(frame_mstore8(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - 6, 1, WORD(U64_FF, 0, 0, 0), 0));
    frame_free(f);

    // zero elements in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    assert_int(frame_mstore8(&f), 1);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // position too high, 64bits
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 2, W1(0), WORD(0, 0, 1, 0));
    assert_int(frame_mstore8(&f), 2);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // gas
    f = frame_init_immed_setup("0x52", 5, 0, 2, W1(0xff), W1(0x00));
    assert_int(frame_mstore8(&f), 2);
    _assert(frame_test_immed(f, IGN, 5, 0, IGN));
    frame_free(f);

    // one element in stack
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 1, W1(0));
    assert_int(frame_mstore8(&f), 3);
    _assert(frame_test_immed(f, IGN, GAS_DEF, 0, IGN));
    frame_free(f);

    // stack full
    f = frame_init_immed_setup("0x52", GAS_DEF, 0, 0);
    for(int i=0; i<1024; i++)
        _assert(!stack_push_immed(&f.s, W1(i)));
    assert_int(frame_mstore8(&f), 0);
    _assert(frame_test_immed(f, IGN, GAS_DEF - 101, IGN, IGN));
    frame_free(f);

    _assert(mem_empty());
}



void test_frame_push()
{
    printf("\n\t%s\t\t", __func__);

    evm_frame_t f = frame_init_immed("0x5f", GAS_DEF);
    assert_int(frame_push(&f), 0);
    _assert(frame_test_immed(f, 1, GAS_DEF - 2, IGN, 1, W1(0)));
    frame_free(f);

    for(int i=1; i<=32; i++)
    {
        char str[70];
        memset(str, 0, 70);
        sprintf(str, "0x%x", 0x5f + i);
        memset(&str[4], 'f', 2*i);

        evm_word_t w = W1(0);
        memset(w.v, 0xff, i);
        
        f = frame_init_immed(str, GAS_DEF);
        assert_int(frame_push(&f), 0);
        _assert(frame_test_immed(f, i+1, GAS_DEF - 3, IGN, 1, w));

        frame_free(f);
    }

    f = frame_init_immed("0x5f", GAS_DEF);
    for(int i=0; i<1024; i++)
        _assert(!stack_push_immed(&f.s, W1(i)));
    assert_int(frame_push(&f), 2);
    _assert(frame_test_immed(f, IGN, GAS_DEF, IGN, IGN));
    frame_free(f);

    f = frame_init_immed("0x5f", 0);
    assert_int(frame_push(&f), 1);
    _assert(frame_test_immed(f, IGN, 0, IGN, IGN));
    frame_free(f);

    f = frame_init_immed("0x5f", GAS_DEF);
    for(int i=0; i<1023; i++)
        _assert(!stack_push_immed(&f.s, W1(i)));
    assert_int(frame_push(&f), 0);
    _assert(frame_test_immed(f, IGN, GAS_DEF - 2, IGN, IGN));
    frame_free(f);

    _assert(mem_empty());
}



void test_frame_return()
{
    printf("\n\t%s\t\t", __func__);

    // common case
    evm_frame_t f = frame_init_immed_setup("0xf3", GAS_DEF, 
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x20), W1(0x00)
    );
    evm_frame_o_t fo = frame_return(&f);
    _assert(frame_o_test_immed(fo, GAS_DEF, "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
    frame_free(f);
    frame_o_free(fo);

    // common case expansion
    f = frame_init_immed_setup("0xf3", GAS_DEF, 
        1, WORD(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
        2, W1(0x20), W1(0x10)
    );
    fo = frame_return(&f);
    _assert(frame_o_test_immed(fo, GAS_DEF - 3, "0x101112131415161718191a1b1c1d1e1f00000000000000000000000000000000"));
    frame_free(f);
    frame_o_free(fo);

    // TODO tests

    // stack 0 elements

    // stack 1 element

    // mem expand too much

    // mem expand infinite

    // normal, return 0

    _assert(mem_empty());
}



void test_frame()
{
    printf("\n%s\t\t", __func__);

    test_frame_init();

    test_frame_stop();

    test_frame_pop();
    test_frame_mload();
    test_frame_mstore();
    test_frame_mstore8();

    test_frame_push();

    test_frame_return();

    _assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_frame();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
