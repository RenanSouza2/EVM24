#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../bytes/debug.h"
#include "../../../word/debug.h"
#include "../../../utils/debug.h"



void test_mem_dry_run(bool show)
{
    TEST_FN_OPEN

    #define TEST_MEM_DRY_RUN(TAG, INITIAL_SIZE, POS, SIZE, RES)   \
    {   \
        TEST_CASE_OPEN(TAG) \
        {   \
            evm_mem_t m = byte_vec_init_zero(); \
            m.size = INITIAL_SIZE;  \
            uint64_t res = mem_dry_run(&m, W1(POS), SIZE);  \
            assert_64(res, RES);    \
        }   \
        TEST_CASE_CLOSE \
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
        uint64_t res = mem_dry_run(&m, WORD(0, 0, 1, 0), 1);
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

    #define TEST_MEM_GET_WORD(TAG, MEM_BEF, POS)    \
    {   \
        TEST_CASE_OPEN(TAG) \
        {   \
            evm_mem_t m = mem_init_immed(ARG_OPEN MEM_BEF); \
            word_t w = mem_get_word(&m, POS);   \
            assert(mem_immed(m, 1, W1(0)));    \
            assert(word_test(w, W1(0)));    \
        }   \
        TEST_CASE_CLOSE \
    }

    TEST_CASE_OPEN(1)
    {
        evm_mem_t m = byte_vec_init_zero();
        word_t w = mem_get_word(&m, 0);
        assert(mem_immed(m, 1, W1(0)));
        assert(word_test(w, W1(0)));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        evm_mem_t m = mem_init_immed(1, W1(0xff));
        word_t w = mem_get_word(&m, 0);
        assert(mem_immed(m, 1, W1(0xff)));
        assert(word_test(w, W1(0xff)));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        evm_mem_t m = mem_init_immed(1, W1(0xff));
        word_t w = mem_get_word(&m, 31);
        assert(mem_immed(m, 2, W1(0xff), W1(0)));
        assert(word_test(w, WORD(U64_FF, 0, 0, 0)));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        evm_mem_t m = mem_init_immed(1, W1(0xff));
        word_t w = mem_get_word(&m, 32);
        assert(mem_immed(m, 2, W1(0xff), W1(0)));
        assert(word_test(w, W1(0)));
        vec_free(&m);
    }
    TEST_CASE_CLOSE

    #undef TEST_MEM_GET_WORD

    TEST_FN_CLOSE
}

void test_mem_get_bytes(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    TEST_CASE_CLOSE

    evm_mem_t m = mem_init_immed(1, W1(0x1234));
    evm_bytes_t b = mem_get_bytes(&m, 0x1e, 2);
    assert(byte_vec_test_immed(b, "0x1234"));
    assert(mem_immed(m, 1, W1(0x1234)));
    vec_free(&b);

    b = mem_get_bytes(&m, 0x40, 0);
    assert(byte_vec_test_immed(b, "0x"));
    assert(mem_immed(m, 1, W1(0x1234)));
    vec_free(&b);

    b = mem_get_bytes(&m, 0x1e, 3);
    assert(byte_vec_test_immed(b, "0x123400"));
    assert(mem_immed(m, 2, W1(0x1234), W1(0)));
    vec_free(&b);
    vec_free(&m);

    TEST_FN_CLOSE
}



void test_mem_set_byte(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    TEST_CASE_CLOSE

    evm_mem_t m = byte_vec_init_zero();
    mem_set_byte(&m, 0, 0xff);
    assert(mem_immed(m, 1, WORD(U64_FF, 0, 0, 0)));

    mem_set_byte(&m, 2, 0xee);
    assert(mem_immed(m, 1, WORD(0xff00ee0000000000, 0, 0, 0)));

    mem_set_byte(&m, 31, 0xdd);
    assert(mem_immed(m, 1, WORD(0xff00ee0000000000, 0, 0, 0xdd)));

    mem_set_byte(&m, 32, 0xcc);
    assert(mem_immed(m, 2,
        WORD(0xff00ee0000000000, 0, 0, 0xdd),
        WORD(0xcc00000000000000, 0, 0, 0)
    ));
    vec_free(&m);

    TEST_FN_CLOSE
}

void test_mem_set_word(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    TEST_CASE_CLOSE

    evm_mem_t m = byte_vec_init_zero();
    word_t w = WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX);
    mem_set_word(&m, 0, &w);
    assert(mem_immed(m, 1, WORD(U64_MAX, U64_MAX, U64_MAX, U64_MAX)));

    w = WORD(0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee);
    mem_set_word(&m, 1, &w);
    assert(mem_immed(m, 2,
        WORD(0xffeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee, 0xeeeeeeeeeeeeeeee),
        WORD(0xee00000000000000, 0, 0, 0)
    ));
    vec_free(&m);

    TEST_FN_CLOSE
}

void test_mem_set_bytes(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    TEST_CASE_CLOSE

    evm_mem_t m = byte_vec_init_zero();
    evm_bytes_t b = byte_vec_init_zero();
    mem_set_bytes(&m, 1024, &b);
    assert(mem_immed(m, 0));

    b = byte_vec_init_immed("0x1234");
    mem_set_bytes(&m, 0, &b);
    assert(mem_immed(m, 1, WORD(0x1234000000000000, 0, 0, 0)));
    vec_free(&b);

    b = byte_vec_init_immed("0x5678");
    mem_set_bytes(&m, 1, &b);
    assert(mem_immed(m, 1, WORD(0x1256780000000000, 0, 0, 0)));
    vec_free(&b);
    vec_free(&m);

    m = byte_vec_init_zero();
    b = byte_vec_init_immed("0x1234");
    mem_set_bytes(&m, 0x1f, &b);
    assert(mem_immed(m, 2, W1(0x12),  WORD(0x3400000000000000, 0, 0, 0)));
    vec_free(&b);
    vec_free(&m);

    TEST_FN_CLOSE
}



void test_mem()
{
    TEST_LIB

    bool show = true;

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
