#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../../word/debug.h"
#include "../../../utils/debug.h"



void test_bytes_get_byte(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        evm_bytes_t b = byte_vec_init_immed("0x");
        byte_t res = bytes_get_byte(&b, 0);
        assert_byte(res, 0);
    }
    TEST_CASE_CLOSE

    for(int i=0; i<32; i++)
    {
        TEST_CASE_OPEN(2 + i)
        {
            evm_bytes_t b = byte_vec_init_immed("0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
            byte_t res = bytes_get_byte(&b, i);
            assert_byte(res, i);
            vec_free(&b);
        }
        TEST_CASE_CLOSE
    }

    TEST_CASE_OPEN(34)
    {
        evm_bytes_t b = byte_vec_init_immed("0xff");
        byte_t res = bytes_get_byte(&b, 1);
        assert_byte(res, 0);
        vec_free(&b);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_bytes_get_word(bool show)
{
    TEST_FN_OPEN

    #define TEST_BYTES_GET_WORD(TAG, BYTES, INDEX, RES) \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            evm_bytes_t b = byte_vec_init_immed(BYTES); \
            word_t w = bytes_get_word(&b, INDEX);       \
            assert(word_test(w, RES));                  \
            vec_free(&b);                               \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_BYTES_GET_WORD(1, "0x", 0, W1(0));
    TEST_BYTES_GET_WORD(2, "0xff", 0, W4(U64_FF, 0, 0, 0));
    TEST_BYTES_GET_WORD(3, "0xff", 1, W1(0));
    TEST_BYTES_GET_WORD(4,
        "0x00000000000000000000000000000000000000000000000000000000000000ff", 0, W1(0xff)
    );
    TEST_BYTES_GET_WORD(5,
        "0x00000000000000000000000000000000000000000000000000000000000000ff", 1, W1(0xff00)
    );
    TEST_BYTES_GET_WORD(6,
        "0x00000000000000000000000000000000000000000000000000000000000000ff", 31, W4(U64_FF, 0, 0, 0)
    );
    TEST_BYTES_GET_WORD(7,
        "0x00000000000000000000000000000000000000000000000000000000000000ff", 32, W1(0)
    );

    #undef TEST_BYTES_GET_WORD

    TEST_FN_CLOSE
}

void test_bytes_get_bytes(bool show)
{
    TEST_FN_OPEN

    #define TEST_BYTES_GET_BYTES(TAG, INDEX, SIZE, RES)             \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_bytes_t b = byte_vec_init_immed("0x0001020304");    \
            evm_bytes_t res = bytes_get_bytes(&b, INDEX, SIZE);     \
            assert(byte_vec_immed(res, RES));                  \
            vec_free(&b);                                           \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_BYTES_GET_BYTES(1, 0, 0, "0x");
    TEST_BYTES_GET_BYTES(2, 0, 6, "0x000102030400");
    TEST_BYTES_GET_BYTES(3, 2, 2, "0x0203");

    TEST_FN_CLOSE
}



void test_bytes()
{
    TEST_LIB

    bool show = false;

    test_bytes_get_byte(show);
    test_bytes_get_word(show);
    test_bytes_get_bytes(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
