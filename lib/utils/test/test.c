#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_utils_cton(bool show)
{
    TEST_FN_OPEN

    char ascii[] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };

    for(uint64_t i = 0; i < 16; i++)
    {
        TEST_CASE_OPEN(i)
        {
            assert_byte(cton(ascii[i]), i);
        }
        TEST_CASE_CLOSE
    }

    char ASCII[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(uint64_t i = 0; i < 6; i++)
    {
        TEST_CASE_OPEN(17 + i)
        {
            assert_byte(cton(ASCII[i]), i + 10);
        }
        TEST_CASE_CLOSE
    }

    TEST_FN_CLOSE
}



void test_utils_uint64_add(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        uint64_t res = uint64_add(1, 2);
        assert_64(res, 3);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        uint64_t res = uint64_add(UINT64_MAX, 2);
        assert_64(res, UINT64_MAX);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint64_get_byte(bool show)
{
    TEST_FN_OPEN

    for(uint64_t i = 0; i < 8; i++)
    {
        TEST_CASE_OPEN(i + 1)
        {
            byte_t res = uint64_get_byte(0x0807060504030201, i);
            assert_byte(res, i + 1);
        }
        TEST_CASE_CLOSE
    }

    TEST_CASE_OPEN(8)
    {
        TEST_REVERT_OPEN
        {
            uint64_get_byte(0x0807060504030201, 8);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(9)
    {
        TEST_REVERT_OPEN
        {
            uint64_get_byte(0x0807060504030201, 9);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint64_set_byte(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT64_SET_BYTE(TAG, U, INDEX, B, RES)   \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            uint64_t res = uint64_set_byte(U, INDEX, B);        \
            assert_64(res, RES);                                \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_UTILS_UINT64_SET_BYTE(1, 0, 0, 0xff, 0xff);
    TEST_UTILS_UINT64_SET_BYTE(2, 0xff, 0, 0xee, 0xee);
    TEST_UTILS_UINT64_SET_BYTE(3, 0, 1, 0xff, 0xff00);
    TEST_UTILS_UINT64_SET_BYTE(4, 0xff, 1, 0xee, 0xeeff);

    for(uint64_t i = 0; i < 8; i++)
        TEST_UTILS_UINT64_SET_BYTE(5 + i, 0, i, 0xff, (uint64_t)0xff << (i << 3));

    #undef TEST_UTILS_UINT64_SET_BYTE

    TEST_CASE_OPEN(13)
    {
        uint64_t u = 0;
        for(uint64_t i = 0; i < 8; i++)
            u = uint64_set_byte(u, i, i + 1);
        assert_64(u, 0x0807060504030201);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(14)
    {
        TEST_REVERT_OPEN
        {
            uint64_set_byte(0x0807060504030201, 8, 1);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(15)
    {
        TEST_REVERT_OPEN
        {
            uint64_set_byte(0x0807060504030201, 9, 1);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint64_get_size(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT64_GET_SIZE(TAG, VALUE, RES) \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            uint64_t res = uint64_get_size(VALUE);      \
            assert_64(res, RES);                        \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_UTILS_UINT64_GET_SIZE(1, 0, 0);
    TEST_UTILS_UINT64_GET_SIZE(2, 1, 1);
    TEST_UTILS_UINT64_GET_SIZE(3, 0xff, 1);
    TEST_UTILS_UINT64_GET_SIZE(4, 0x100, 2);
    TEST_UTILS_UINT64_GET_SIZE(5, U64_FF, 8);
    TEST_UTILS_UINT64_GET_SIZE(6, U64_MAX, 8);

    #undef TEST_UTILS_UINT64_GET_SIZE

    TEST_FN_CLOSE
}

void test_utils_uint64_init_byte_arr(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT64_INIT_BYTE_ARR(TAG, BYTES, RES)    \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            byte_vec_t b = byte_vec_init_immed(BYTES);          \
            uint64_t res = uint64_init_byte_arr(b.size, b.arr); \
            assert_64(res, RES);                                \
            vec_free(&b);                                       \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_UTILS_UINT64_INIT_BYTE_ARR(1, "0xff", 0xff);
    TEST_UTILS_UINT64_INIT_BYTE_ARR(2, "0xffee", 0xffee);
    TEST_UTILS_UINT64_INIT_BYTE_ARR(3, "0xffffffffffffffff", U64_MAX);
    TEST_UTILS_UINT64_INIT_BYTE_ARR(4, "0x0807060504030201", 0x0807060504030201);

    #undef TEST_UTILS_UINT64_INIT_BYTE_ARR

    TEST_CASE_OPEN(5)
    {
        byte_vec_t b = byte_vec_init_immed("0x00010203040506070809");
        TEST_REVERT_OPEN
        {
            uint64_init_byte_arr(b.size, b.arr);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint128_to_uint64(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT128_TO_UINT64(TAG, U128, RES)    \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            uint64_t res = uint128_to_uint64(U128);         \
            assert_64(res, RES);                            \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_UTILS_UINT128_TO_UINT64(1, 0, 0);
    TEST_UTILS_UINT128_TO_UINT64(2, 1, 1);
    TEST_UTILS_UINT128_TO_UINT64(3, U64_MAX - 1, U64_MAX - 1);
    TEST_UTILS_UINT128_TO_UINT64(4, U64_MAX, U64_MAX);
    TEST_UTILS_UINT128_TO_UINT64(5, U128_1, U64_MAX);
    TEST_UTILS_UINT128_TO_UINT64(6, U128_MAX, U64_MAX);

    #undef TEST_UTILS_UINT128_TO_UINT64

    TEST_FN_CLOSE
}



void test_utils_byte_vec_init_zero(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        byte_vec_t b = byte_vec_init_zero();
        assert(b.size == 0);
        assert(b.arr == NULL);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_byte_vec_init_uint64(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_BYTE_VEC_INIT_UINT64(TAG, U64, RES)  \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            byte_vec_t b = byte_vec_init_uint64(U64);       \
            assert(byte_vec_immed(b, RES));                 \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_UTILS_BYTE_VEC_INIT_UINT64(1, 0, "0x");
    TEST_UTILS_BYTE_VEC_INIT_UINT64(2, 1, "0x01");
    TEST_UTILS_BYTE_VEC_INIT_UINT64(3, 0xff, "0xff");
    TEST_UTILS_BYTE_VEC_INIT_UINT64(4, 0x100, "0x0100");
    TEST_UTILS_BYTE_VEC_INIT_UINT64(5, U64_FF, "0xff00000000000000");
    TEST_UTILS_BYTE_VEC_INIT_UINT64(6, U64_MAX, "0xffffffffffffffff");

    #undef TEST_UTILS_BYTE_VEC_INIT_UINT64

    TEST_FN_CLOSE
}

void test_utils_byte_vec_init_byte_arr(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(TAG, BYTE_IN, OFFSET, SIZE, RES)  \
    {                                                                           \
        TEST_CASE_OPEN(TAG)                                                     \
        {                                                                       \
            byte_vec_t b = byte_vec_init_immed(BYTE_IN);                        \
            byte_vec_t b_res = byte_vec_init_byte_arr(SIZE, &b.arr[OFFSET]);    \
            assert(byte_vec_immed(b_res, RES));                                 \
            vec_free(&b);                                                       \
        }                                                                       \
        TEST_CASE_CLOSE                                                         \
    }

    TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(1, "0x", 0, 0, "0x");
    TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(2, "0x00", 0, 1, "0x00");
    TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(3, "0x0001", 0, 1, "0x00");
    TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(4, "0x0001", 1, 1, "0x01");
    TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR(5, "0x0001", 0, 2, "0x0001");

    #undef TEST_UTILS_BYTE_VEC_INIT_BYTE_ARR

    TEST_FN_CLOSE
}

void test_utils_byte_vec_concat(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_BYTE_VEC_CONCAT(TAG, BYTE_1, BYTE_2, RES)    \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            byte_vec_t b_1 = byte_vec_init_immed(BYTE_1);           \
            byte_vec_t b_2 = byte_vec_init_immed(BYTE_2);           \
            b_1 = byte_vec_concat(&b_1, &b_2);                      \
            assert(byte_vec_immed(b_1, RES));                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_UTILS_BYTE_VEC_CONCAT(1, "0x", "0x", "0x");
    TEST_UTILS_BYTE_VEC_CONCAT(2, "0x01", "0x", "0x01");
    TEST_UTILS_BYTE_VEC_CONCAT(3, "0x", "0x02", "0x02");
    TEST_UTILS_BYTE_VEC_CONCAT(4, "0x01", "0x02", "0x0102");

    #undef TEST_UTILS_BYTE_VEC_CONCAT

    TEST_FN_CLOSE
}



void test_utils_uint64_vec_init_zero(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        uint64_vec_t u = uint64_vec_init_zero();
        assert(u.size == 0);
        assert(u.arr == NULL);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint64_vec_init(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        uint64_vec_t u = uint64_vec_init(0);
        assert(u.size == 0);
        assert(u.arr == NULL);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        uint64_vec_t u = uint64_vec_init(1);
        assert(u.size == 1);
        assert(u.arr != NULL);
        assert(u.arr[0] == 0);
        vec_free(&u);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_utils_uint64_vec_has_uint64(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT64_VEC_HAS_UINT64(TAG, VEC, U64, RES)    \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            uint64_vec_t vec = uint64_vec_init_immed(ARG_OPEN VEC); \
            bool res = uint64_vec_has_uint64(&vec, U64);            \
            assert(res == RES);                                     \
            vec_free(&vec);                                         \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_UTILS_UINT64_VEC_HAS_UINT64(1, (1, 1), 0, false);
    TEST_UTILS_UINT64_VEC_HAS_UINT64(2, (1, 1), 1, true);
    TEST_UTILS_UINT64_VEC_HAS_UINT64(3, (1, 1), 2, false);

    TEST_UTILS_UINT64_VEC_HAS_UINT64(4, (2, 0, 2), 0, true);
    TEST_UTILS_UINT64_VEC_HAS_UINT64(5, (2, 0, 2), 1, false);
    TEST_UTILS_UINT64_VEC_HAS_UINT64(6, (2, 0, 2), 2, true);
    TEST_UTILS_UINT64_VEC_HAS_UINT64(7, (2, 0, 2), 3, false);

    #undef TEST_UTILS_UINT64_VEC_HAS_UINT64

    TEST_FN_CLOSE
}



void test_utils()
{
    TEST_LIB

    bool show = false;

    test_utils_cton(show);

    test_utils_uint64_add(show);
    test_utils_uint64_get_byte(show);
    test_utils_uint64_set_byte(show);
    test_utils_uint64_get_size(show);
    test_utils_uint64_init_byte_arr(show);
    test_utils_uint128_to_uint64(show);

    test_utils_byte_vec_init_zero(show);
    test_utils_byte_vec_init_uint64(show);
    test_utils_byte_vec_init_byte_arr(show);
    test_utils_byte_vec_concat(show);

    test_utils_uint64_vec_init_zero(show);
    test_utils_uint64_vec_init(show);
    test_utils_uint64_vec_has_uint64(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
