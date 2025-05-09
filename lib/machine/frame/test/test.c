#include <string.h>

#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../mem/debug.h"
#include "../../stack/head/debug.h"
#include "../../gas/header.h"
#include "../../../utils/debug.h"



void test_frame_get_jumpdest(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_GET_JUMPDEST(TAG, CODE, RES)                 \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            uint64_vec_t jumpdest = frame_get_jumpdest_immed(CODE); \
            assert(uint64_vec_immed(jumpdest, ARG_OPEN RES));       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_GET_JUMPDEST(1, "0x", (0));
    TEST_FRAME_GET_JUMPDEST(2, "0x00", (0));
    TEST_FRAME_GET_JUMPDEST(3, "0x5b", (1, 0));
    TEST_FRAME_GET_JUMPDEST(4, "0x605b", (0));
    TEST_FRAME_GET_JUMPDEST(5, "0x005b", (1, 1));

    #undef TEST_FRAME_GET_JUMPDEST

    TEST_FN_CLOSE
}

void test_frame_init(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        byte_vec_t code = byte_vec_init_immed("0x");
        evm_frame_t f = frame_init(code, GAS_DEF);
        assert_64(f.pc, 0);
        assert_64(f.gas, GAS_DEF);
        assert(byte_vec_immed(f.code, "0x"));
        assert(uint64_vec_immed(f.jumpdest, 0));
        assert(mem_immed(f.m, 0));
        assert(stack_immed(f.s, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        byte_vec_t code = byte_vec_init_immed("0x5b");
        evm_frame_t f = frame_init(code, GAS_DEF);
        assert_64(f.pc, 0);
        assert_64(f.gas, GAS_DEF);
        assert(byte_vec_immed(f.code, "0x5b"));
        assert(uint64_vec_immed(f.jumpdest, 1, 0));
        assert(mem_immed(f.m, 0));
        assert(stack_immed(f.s, 0));
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_frame_get_opcode(bool show)
{
    TEST_FN_OPEN

    for(int i=0; i<4; i++)
    {
        TEST_CASE_OPEN(i+1)
        {
            evm_frame_t f = frame_init_immed("0x00010203", i, GAS_DEF, 0, 0);
            byte_t res = frame_get_opcode(&f);
            assert_byte(res, i);
            frame_free(&f);
        }
        TEST_CASE_CLOSE
    }

    TEST_FN_CLOSE
}

void test_frame_push_uint64(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        evm_frame_t f = frame_init_immed("0x", 0, GAS_DEF, 0, 0);
        uint64_t err = frame_push_uint64(&f, 1);
        assert_64(err, 0);
        assert(frame_immed(f, "0x", 0, GAS_DEF - 2, 0, 1, W1(1)));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        evm_frame_t f = frame_init_immed("0x", 0, 0, 0, 0);
        uint64_t err = frame_push_uint64(&f, 1);
        assert_64(err, 1);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        evm_frame_t f = frame_init_immed("0x", 0, GAS_DEF, 0, 0);
        frame_stack_populate(&f, 1024);
        uint64_t err = frame_push_uint64(&f, 2);
        assert_64(err, 0x12);
        frame_free(&f);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_frame_STOP(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        evm_frame_t f = frame_init_immed("0x", 0, GAS_DEF, 0, 0);
        evm_frame_o_t fo = frame_STOP(&f);
        assert(frame_o_immed(fo, true, GAS_DEF, "0x"));
        frame_free(&f);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_frame_POP(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_POP(TAG, FRAME_BEF, FRAME_AFT)               \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME_BEF);   \
            bool res = frame_POP(&f);                               \
            assert_64(res, 0);                                      \
            assert(frame_immed(f, ARG_OPEN FRAME_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_POP(1,
        ("0x50", 0, GAS_DEF, 0, 2, W1(1), W1(2)),
        ("0x50", 0, GAS_DEF - G_base, 0, 1, W1(1))
    );
    TEST_FRAME_POP(2,
        ("0x50", 0, GAS_DEF, 0, 1, W1(1)),
        ("0x50", 0, GAS_DEF - G_base, 0, 0)
    );

    #undef TEST_FRAME_POP

    #define TEST_FRAME_POP(TAG, FRAME, ERR)                     \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            uint64_t err = frame_POP(&f);                       \
            assert_64(err, ERR);                                \
            frame_free(&f);                                     \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_POP(3, ("0x50", 0, 0, 0, 0), 1);
    TEST_FRAME_POP(4, ("0x50", 0, GAS_DEF, 0, 0), 0x12);

    #undef TEST_FRAME_POP

    TEST_FN_CLOSE
}

void test_frame_MLOAD(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_MLOAD(TAG, FRAME_BEF, FRAME_AFT)             \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME_BEF);   \
            uint64_t err = frame_MLOAD(&f);                         \
            assert_64(err, 0);                                      \
            assert(frame_immed(f, ARG_OPEN FRAME_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_MLOAD(1,
        ("0x51", 0, GAS_DEF, 1, W1(0xff), 1, W1(0x00)),
        ("0x51", 0, GAS_DEF - G_very_low, 1, W1(0xff), 1, W1(0xff))
    );
    TEST_FRAME_MLOAD(2,
        ("0x51", 0, GAS_DEF, 1, W1(0xff), 1, W1(0x10)),
        ("0x51", 0, GAS_DEF - 6, 2, W1(0xff), W1(0), 1, W4(0, 0xff, 0, 0))
    );
    TEST_FRAME_MLOAD(3,
        ("0x51", 0, GAS_DEF, 0, 1, W1(0x00)),
        ("0x51", 0, GAS_DEF - 6, 1, W1(0), 1, W1(0))
    );

    #undef TEST_FRAME_MLOAD

    #define TEST_FRAME_MLOAD(TAG, FRAME, ERR)                   \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            uint64_t err = frame_MLOAD(&f);                     \
            assert_64(err, ERR);                                \
            frame_free(&f);                                     \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_MLOAD(4, ("0x51", 0, GAS_DEF, 1, W1(0xff), 0), 0x11);
    TEST_FRAME_MLOAD(5, ("0x51", 0, 0, 1, W1(0xff), 1, W1(0x00)), 2);
    TEST_FRAME_MLOAD(6, ("0x51", 0, GAS_DEF, 0, 1, W1(U64_MAX)), 2);

    #undef TEST_FRAME_MLOAD

    TEST_FN_CLOSE
}

void test_frame_MSTORE(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_MSTORE(TAG, FRAME_BEF, FRAME_AFT)            \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME_BEF);   \
            uint64_t err = frame_MSTORE(&f);                        \
            assert_64(err, 0);                                      \
            assert(frame_immed(f, ARG_OPEN FRAME_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_MSTORE(1,
        ("0x52", 0, GAS_DEF, 0, 2, W1(0xff), W1(0x00)),
        ("0x52", 0, GAS_DEF - 6, 1, W1(0xff), 0)
    );

    #undef TEST_FRAME_MSTORE

    #define TEST_FRAME_MSTORE(TAG, FRAME, ERR)                  \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            uint64_t err = frame_MSTORE(&f);                    \
            assert_64(err, ERR);                                \
            frame_free(&f);                                     \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_MSTORE(2, ("0x52", 0, GAS_DEF, 0, 0), 0x11);
    TEST_FRAME_MSTORE(3, ("0x52", 0, GAS_DEF, 0, 1, W1(0)), 0x12);
    TEST_FRAME_MSTORE(4, ("0x52", 0, GAS_DEF, 0, 2, W1(0), W1(U64_MAX)), 3);
    TEST_FRAME_MSTORE(5, ("0x52", 0, 0, 0, 2, W1(0xff), W1(0x00)), 3);

    TEST_FN_CLOSE
}

void test_frame_MSTORE8(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_MSTORE8(TAG, FRAME_BEF, FRAME_AFT)           \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME_BEF);   \
            uint64_t err = frame_MSTORE8(&f);                       \
            assert_64(err, 0);                                      \
            assert(frame_immed(f, ARG_OPEN FRAME_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_MSTORE8(1,
        ("0x53", 0, GAS_DEF, 0, 2, W1(0xff), W1(0x00)),
        ("0x53", 0, GAS_DEF - 6, 1, W4(U64_FF, 0, 0, 0), 0)
    );
    TEST_FRAME_MSTORE8(2,
        ("0x53", 0, GAS_DEF, 0, 2, W1(0xffff), W1(0x00)),
        ("0x53", 0, GAS_DEF - 6, 1, W4(U64_FF, 0, 0, 0), 0)
    );

    #undef TEST_FRAME_MSTORE8

    #define TEST_FRAME_MSTORE8(TAG, FRAME, ERR)                 \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            uint64_t err = frame_MSTORE8(&f);                   \
            assert_64(err, ERR);                                \
            frame_free(&f);                                     \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_MSTORE8(3, ("0x53", 0, GAS_DEF, 0, 0), 0x11);
    TEST_FRAME_MSTORE8(4, ("0x53", 0, GAS_DEF, 0, 1, W1(0)), 0x12);
    TEST_FRAME_MSTORE8(5, ("0x53", 0, GAS_DEF, 0, 2, W1(0), W1(U64_MAX)), 3);
    TEST_FRAME_MSTORE8(6, ("0x53", 0, 0, 0, 2, W1(0xff), W1(0x00)), 3);

    #undef TEST_FRAME_MSTORE8

    TEST_FN_CLOSE
}



void test_frame_PUSH(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_PUSH(TAG, FRAME_BEF, FRAME_AFT)              \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME_BEF);   \
            byte_t opcode = frame_get_opcode(&f);                   \
            uint64_t err = frame_PUSH(&f, opcode);                  \
            assert_64(err, 0);                                      \
            assert(frame_immed(f, ARG_OPEN FRAME_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FRAME_PUSH(1,
        ("0x5f", 0, GAS_DEF, 0, 0),
        ("0x5f", 1, GAS_DEF - G_base, 0, 1, W1(0))
    );

    for(int i=1; i<=32; i++)
    {
        char code[70];
        memset(code, 0, 70);
        sprintf(code, "0x%x", 0x5f + i);
        memset(&code[4], 'f', 2*i);

        word_t w = W1(0);
        memset(w.arr, 0xff, i);

        TEST_FRAME_PUSH(i,
            (code, 0, GAS_DEF, 0, 0),
            (code, 1 + i, GAS_DEF - G_very_low, 0, 1, w)
        );
    }

    #undef TEST_FRAME_PUSH

    TEST_CASE_OPEN(33)
    {
        evm_frame_t f = frame_init_immed("0x5f", 0, GAS_DEF, 0, 0);
        frame_stack_populate(&f, 1023);
        byte_t opcode = frame_get_opcode(&f);
        uint64_t err = frame_PUSH(&f, opcode);
        evm_frame_t f_exp = frame_init_immed("0x5f", 1, GAS_DEF - G_base, 0, 0);
        frame_stack_populate(&f_exp, 1023);
        frame_stack_populate(&f_exp, 1);
        assert_64(err, 0);
        assert(frame_test(f, f_exp));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(34)
    {
        evm_frame_t f = frame_init_immed("0x5f", 0, GAS_DEF, 0, 0);
        frame_stack_populate(&f, 1024);
        byte_t opcode = frame_get_opcode(&f);
        uint64_t err = frame_PUSH(&f, opcode);
        assert_64(err, 0x12);
        frame_free(&f);
    }
    TEST_CASE_CLOSE

    #define TEST_FRAME_PUSH(TAG, FRAME, ERR)                    \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            byte_t opcode = frame_get_opcode(&f);               \
            uint64_t err = frame_PUSH(&f, opcode);              \
            assert_64(err, ERR);                                \
            frame_free(&f);                                     \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_PUSH(34, ("0x5f", 0, 0, 0, 0), 1);

    #undef TEST_FRAME_PUSH

    TEST_FN_CLOSE
}



void test_frame_RETURN(bool show)
{
    TEST_FN_OPEN

    #define TEST_FRAME_RETURN(TAG, FRAME, RES)                  \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            evm_frame_t f = frame_init_immed(ARG_OPEN FRAME);   \
            evm_frame_o_t fo = frame_RETURN(&f);                \
            assert(frame_o_immed(fo, ARG_OPEN RES));            \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_FRAME_RETURN(1,
        (
            "0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            2, W1(0x20), W1(0x00)
        ),
        (true, GAS_DEF, "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")
    );
    TEST_FRAME_RETURN(2,
        ("0xf3", 0, GAS_DEF, 0, 2, W1(0x20), W1(0x00)),
        (true, GAS_DEF - G_very_low, "0x0000000000000000000000000000000000000000000000000000000000000000")
    );
    TEST_FRAME_RETURN(3,
        ("0xf3", 0, GAS_DEF, 0, 2, W1(0x00), W1(0x00)),
        (true, GAS_DEF, "0x")
    );
    TEST_FRAME_RETURN(4,
        ("0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            1, W1(0x20)
        ),
        (false, 0, "0x")
    );
    TEST_FRAME_RETURN(5,
        (
            "0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            0
        ),
        (false, 0, "0x")
    );
    TEST_FRAME_RETURN(6,
        (
            "0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            2, W1(0x20), W1(U64_MAX)
        ),
        (false, 0, "0x")
    );
    TEST_FRAME_RETURN(7,
        (
            "0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            2, W1(0x20), W1(U64_MAX)
        ),
        (false, 0, "0x")
    );
    TEST_FRAME_RETURN(8,
        (
            "0xf3", 0, GAS_DEF,
            1, W4(0x0001020304050607, 0x08090a0b0c0d0e0f, 0x1011121314151617, 0x18191a1b1c1d1e1f),
            2, W1(0x00), W1(U64_MAX)
        ),
        (true, GAS_DEF, "0x")
    );

    #undef TEST_FRAME_RETURN

    TEST_FN_CLOSE
}



void test_frame()
{
    TEST_LIB

    bool show = true;

    test_frame_get_jumpdest(show);
    test_frame_init(show);

    test_frame_get_opcode(show);
    test_frame_push_uint64(show);

    test_frame_STOP(show);

    test_frame_POP(show);
    test_frame_MLOAD(show);
    test_frame_MSTORE(show);
    test_frame_MSTORE8(show);

    test_frame_PUSH(show);

    test_frame_RETURN(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_frame();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
