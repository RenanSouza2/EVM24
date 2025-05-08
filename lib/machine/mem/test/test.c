#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../bytes/debug.h"
#include "../../../word/debug.h"
#include "../../../utils/debug.h"



void test_mem_dry_run(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_DRY_RUN(TAG, INITIAL_SIZE, INDEX, SIZE, RES)   \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_mem_t m = byte_vec_init_zero();                     \
            m.size = INITIAL_SIZE;                                  \
            uint64_t res = mem_dry_run(&m, W1(INDEX), SIZE);        \
            assert_64(res, RES);                                    \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_MEM_DRY_RUN(1, 0,   1, 0,  0);
    TEST_MEM_DRY_RUN(2, 0,   0, 1,  3);
    TEST_MEM_DRY_RUN(3, 0,  30, 1,  3);
    TEST_MEM_DRY_RUN(4, 0,  31, 1,  3);
    TEST_MEM_DRY_RUN(5, 0,  63, 1,  6);
    TEST_MEM_DRY_RUN(6, 0,  95, 1,  9);
    TEST_MEM_DRY_RUN(7, 0, 703, 1, 66);
    TEST_MEM_DRY_RUN(8, 0, 704, 1, 70);

    TEST_MEM_DRY_RUN( 9, 64,   0, 1,  0);
    TEST_MEM_DRY_RUN(10, 64,  30, 1,  0);
    TEST_MEM_DRY_RUN(11, 64,  31, 1,  0);
    TEST_MEM_DRY_RUN(12, 64,  63, 1,  0);
    TEST_MEM_DRY_RUN(13, 64,  95, 1,  3);
    TEST_MEM_DRY_RUN(14, 64, 703, 1, 60);
    TEST_MEM_DRY_RUN(15, 64, 704, 1, 64);

    TEST_MEM_DRY_RUN(16, 736,    0, 1,  0);
    TEST_MEM_DRY_RUN(17, 736,  735, 1,  0);
    TEST_MEM_DRY_RUN(18, 736,  767, 1,  3);
    TEST_MEM_DRY_RUN(19, 736, 1023, 1, 28);

    TEST_MEM_DRY_RUN(20, 736, U64_MAX - 1, 1, U64_MAX);
    TEST_MEM_DRY_RUN(21, 736, U64_MAX , 1, U64_MAX);
    TEST_MEM_DRY_RUN(22, 736, 0xfffffffffffe, 1, U64_MAX);

    TEST_CASE_OPEN(23)
    {
        evm_mem_t m = byte_vec_init_zero();
        m.size = 736;
        uint64_t res = mem_dry_run(&m, W4(0, 0, 1, 0), 1);
        assert_64(res, U64_MAX);
    }
    TEST_CASE_CLOSE

    #undef TEST_MEM_DRY_RUN

    TEST_FN_CLOSE
}

void test_mem_expand(bool show)
{
    TEST_FN_OPEN

    for(uint64_t i=0; i<4; i++)
    {
        TEST_CASE_OPEN(33 * i + 1)
        {
            evm_mem_t m = byte_vec_init_zero();
            mem_expand(&m, 32 * i);
            assert(m.size == 32 * i);
            vec_free(&m);
        }
        TEST_CASE_CLOSE

        for(uint64_t j=1; j<32; j++)
        {
            TEST_CASE_OPEN(33 * i + j + 1)
            {
                evm_mem_t m = byte_vec_init_zero();
                mem_expand(&m, 32 * i + j);
                assert(m.size == 32 * (i + 1));
                vec_free(&m);
            }
            TEST_CASE_CLOSE
        }
    }

    TEST_FN_CLOSE
}



void test_mem_get_word(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_GET_WORD(TAG, MEM_BEF, INDEX, WORD, MEM_AFT)   \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF);         \
            word_t w = mem_get_word(&m, INDEX);                     \
            assert(word_test(w, WORD));                             \
            assert(mem_immed(m, ARG_OPEN MEM_AFT));                 \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_MEM_GET_WORD(1, (0), 0, W1(0), (1, W1(0)));
    TEST_MEM_GET_WORD(2, (1, W1(0xff)), 0, W1(0xff), (1, W1(0xff)));
    TEST_MEM_GET_WORD(3, (1, W1(0xff)), 31, W4(U64_FF, 0, 0, 0), (2, W1(0xff), W1(0)));
    TEST_MEM_GET_WORD(4, (1, W1(0xff)), 32, W1(0), (2, W1(0xff), W1(0)));

    #undef TEST_MEM_GET_WORD

    TEST_FN_CLOSE
}

void test_mem_get_bytes(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_GET_BYTES(TAG, MEM_BEF, INDEX, SIZE, RES, MEM_AFT) \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF);             \
            evm_bytes_t b = mem_get_bytes(&m, INDEX, SIZE);             \
            assert(byte_vec_immed(b, RES));                        \
            assert(mem_immed(m, ARG_OPEN MEM_AFT));                     \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_MEM_GET_BYTES(1, (1, W1(0x1234)), 0x1e, 2, "0x1234", (1, W1(0x1234)));
    TEST_MEM_GET_BYTES(2, (1, W1(0x1234)), 0x40, 0, "0x", (1, W1(0x1234)));
    TEST_MEM_GET_BYTES(3, (1, W1(0x1234)), 0x1e, 3, "0x123400", (2, W1(0x1234), W1(0)));

    TEST_FN_CLOSE
}



void test_mem_set_byte(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_SET_BYTE(TAG, MEM_BEF, INDEX, BYTE, MEM_AFT)   \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF);         \
            mem_set_byte(&m, INDEX, BYTE);                          \
            assert(mem_immed(m, ARG_OPEN MEM_AFT));                 \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_MEM_SET_BYTE(1, (0), 0, 0xff, (1, W4(U64_FF, 0, 0, 0)));
    TEST_MEM_SET_BYTE(2,
        (1, W4(U64_FF, 0, 0, 0)),
        2, 0xee,
        (1, W4(0xff00ee0000000000, 0, 0, 0))
    );
    TEST_MEM_SET_BYTE(3,
        (1, W4(0xff00ee0000000000, 0, 0, 0)),
        31, 0xdd,
        (1, W4(0xff00ee0000000000, 0, 0, 0xdd))
    );
    TEST_MEM_SET_BYTE(4,
        (1, W4(0xff00ee0000000000, 0, 0, 0xdd)),
        32, 0xcc,
        (
            2,
            W4(0xff00ee0000000000, 0, 0, 0xdd),
            W4(0xcc00000000000000, 0, 0, 0)
        )
    );

    #undef TEST_MEM_SET_WORD

    TEST_FN_CLOSE
}

void test_mem_set_word(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_SET_WORD(TAG, MEM_BEF, INDEX, WORD, MEM_AFT)   \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF);         \
            word_t w = WORD;                                        \
            mem_set_word(&m, INDEX, &w);                            \
            assert(mem_immed(m, ARG_OPEN MEM_AFT));                 \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_MEM_SET_WORD(1,
        (0),
        0, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX),
        (1, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX))
    );
    TEST_MEM_SET_WORD(2,
        (1, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX)),
        1, W4(0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee),
        (
            2,
            W4(0xffeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee),
            W4(0xee00000000000000, 0, 0, 0)
        )
    );
    TEST_MEM_SET_WORD(3, (0), 0, W1(0xff), (1, W1(0xff)));

    #undef TEST_MEM_SET_WORD

    TEST_FN_CLOSE
}

void test_mem_set_bytes(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_SET_BYTES(TAG, MEM_BEF, INDEX, BYTES, MEM_AFT) \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF);         \
            evm_bytes_t b = byte_vec_init_immed(BYTES);             \
            mem_set_bytes(&m, INDEX, &b);                           \
            assert(mem_immed(m, ARG_OPEN MEM_AFT));                 \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_MEM_SET_BYTES(1, (0), 1024, "0x", (0));
    TEST_MEM_SET_BYTES(2, (0), 0, "0x1234", (1, W4(0x1234000000000000, 0, 0, 0)));
    TEST_MEM_SET_BYTES(3,
        (1, W4(0x1234000000000000, 0, 0, 0)),
        1, "0x5678",
        (1, W4(0x1256780000000000, 0, 0, 0))
    );
    TEST_MEM_SET_BYTES(2, (0), 0x1f, "0x1234", (2, W1(0x12),  W4(0x3400000000000000, 0, 0, 0)));

    TEST_FN_CLOSE
}



void test_mem()
{
    TEST_LIB

    bool show = false;

    test_mem_dry_run(show);
    test_mem_expand(show);

    test_mem_get_word(show);
    test_mem_get_bytes(show);

    test_mem_set_byte(show);
    test_mem_set_word(show);
    test_mem_set_bytes(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
