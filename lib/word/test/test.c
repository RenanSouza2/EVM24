#include <stdio.h>

#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../machine/bytes/debug.h"
#include "../../utils/debug.h"



void test_word_mem_size(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        assert(sizeof(word_t) == 32);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_word_init_bytes(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_INIT_BYTES(TAG, BYTES, RES)       \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            evm_bytes_t b = byte_vec_init_immed(BYTES); \
            word_t w = word_init_bytes(&b);             \
            assert(word_test(w, RES));                  \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_WORD_INIT_BYTES(1, "0x", W1(0));
    TEST_WORD_INIT_BYTES(2, "0xff", W1(0xff));
    TEST_WORD_INIT_BYTES(3, "0xffee", W1(0xffee));
    TEST_WORD_INIT_BYTES(4, "0xff0000000000000000", W4(0, 0, 0xff, 0));

    #undef TEST_WORD_INIT_BYTES

    TEST_FN_CLOSE
}



void test_word_is_uint_64(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_IS_UINT64(TAG, WORD, RES) \
    {                                           \
        TEST_CASE_OPEN(TAG)                     \
        {                                       \
            word_t w = WORD;                    \
            bool res = word_is_uint64(&w);      \
            assert(res == RES);                 \
        }                                       \
        TEST_CASE_CLOSE                         \
    }

    TEST_WORD_IS_UINT64(1, W1(0), true);
    TEST_WORD_IS_UINT64(2, W1(U64_MAX), true);
    TEST_WORD_IS_UINT64(2, W4(0, 0, 1, 0), false);
    TEST_WORD_IS_UINT64(2, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX), false);

    #undef TEST_WORD_IS_UINT64

    TEST_FN_CLOSE
}

void test_word_eq(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_EQ(TAG, WORD, RES)    \
    {                                       \
        TEST_CASE_OPEN(TAG)                 \
        {                                   \
            word_t w1 = W4(4, 3, 2, 1);     \
            word_t w2 = WORD;               \
            bool res = word_eq(&w1, &w2);   \
            assert(res == RES);             \
        }                                   \
        TEST_CASE_CLOSE                     \
    }

    TEST_WORD_EQ(1, W4(4, 3, 2, 1), true);
    TEST_WORD_EQ(2, W4(4, 3, 2, 0), false);
    TEST_WORD_EQ(3, W4(4, 3, 0, 1), false);
    TEST_WORD_EQ(4, W4(4, 0, 2, 1), false);
    TEST_WORD_EQ(5, W4(0, 3, 2, 1), false);

    #undef TEST_WORD_EQ

    TEST_FN_CLOSE
}

void test_word_add_uint64(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_ADD_UINT64(TAG, WORD, INDEX, U64, RES)    \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            word_t w = WORD;                                    \
            word_add_uint64(&w, INDEX, U64);                    \
            assert(word_test(w, RES));                          \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_WORD_ADD_UINT64(1, W4(4, 3, 2, 1), 0, 1, W4(4, 3, 2, 2));
    TEST_WORD_ADD_UINT64(2, W4(4, 3, 2, 1), 1, 1, W4(4, 3, 3, 1));
    TEST_WORD_ADD_UINT64(3, W4(4, 3, 2, 1), 2, 1, W4(4, 4, 2, 1));
    TEST_WORD_ADD_UINT64(4, W4(4, 3, 2, 1), 3, 1, W4(5, 3, 2, 1));
    TEST_WORD_ADD_UINT64(5, W4(4, 3, 2, 1), 4, 1, W4(4, 3, 2, 1));
    TEST_WORD_ADD_UINT64(6, W1(0xffffffffff), 0, 0x1f, W1(0x1000000001e));
    TEST_WORD_ADD_UINT64(7, W1(U64_MAX), 0, 1, W4(0, 0, 1, 0));
    TEST_WORD_ADD_UINT64(8, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX), 0, 1, W1(0));

    #undef TEST_WORD_ADD_UINT64

    TEST_FN_CLOSE
}

void test_word_set_byte(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_SET_BYTE(TAG, WORD, INDEX, BYTE, RES) \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            word_t w = WORD;                                \
            word_set_byte(&w, INDEX, BYTE);                 \
            assert(word_test(w, RES));                      \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_WORD_SET_BYTE(1, W1(0), 0, 0xff, W1(0xff));
    TEST_WORD_SET_BYTE(2, W1(0), 1, 0xff, W1(0xff00));
    TEST_WORD_SET_BYTE(3, W1(0), 7, 0xff, W1(U64_FF));
    TEST_WORD_SET_BYTE(4, W1(0), 8, 0xff, W4(0, 0, 0xff, 0));
    TEST_WORD_SET_BYTE(5, W1(0),31, 0xff, W4(U64_FF, 0, 0, 0));
    TEST_WORD_SET_BYTE(6, W1(1), 0, 0xff, W1(0xff));
    TEST_WORD_SET_BYTE(7, W1(1), 1, 0xff, W1(0xff01));
    TEST_WORD_SET_BYTE(8, W1(1), 7, 0xff, W1(U64_FF | 0x01));
    TEST_WORD_SET_BYTE(9, W1(1), 8, 0xff, W4(0, 0, 0xff, 1));
    TEST_WORD_SET_BYTE(10, W1(1),31, 0xff, W4(U64_FF, 0, 0, 1));

    #undef TEST_WORD_SET_BYTE

    TEST_FN_CLOSE
}



void test_word_add(bool show)
{
    TEST_FN_OPEN

    #define TEST_WORD_ADD(TAG, WORD_1, WORD_2, RES) \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            word_t w1 = WORD_1;                     \
            word_t w2 = WORD_2;                     \
            word_t w = word_add(&w1, &w2);          \
            assert(word_test(w, RES));              \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_WORD_ADD(1, W4(4, 3, 2, 1), W4(1, 2, 3, 4), W4(5, 5, 5, 5));
    TEST_WORD_ADD(2, W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX), W4(0, 0, 1, 0), W1(U64_MAX));
    TEST_WORD_ADD(3,
        W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX),
        W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX),
        W4(U64_MAX, U64_MAX, U64_MAX, U64_MAX - 1)
    );

    TEST_FN_CLOSE
}



void test_word()
{
    TEST_LIB

    bool show = false;

    test_word_mem_size(show);
    test_word_init_bytes(show);

    test_word_is_uint_64(show);
    test_word_eq(show);
    test_word_add_uint64(show);
    test_word_set_byte(show);

    test_word_add(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_word();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
