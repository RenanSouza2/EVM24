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

void test_utils_byte_vec_init_immed(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_BYTRE_VEC_INIT_IMMED(TAG, BYTES, SIZE, RES)  \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            byte_vec_t b = byte_vec_init_immed(BYTES);              \
            byte_t arr[] = {ARG_OPEN RES};                          \
            byte_vec_t b_2 = (byte_vec_t)                           \
            {                                                       \
                .size = SIZE,                                       \
                .arr = arr                                          \
            };                                                      \
            assert(byte_vec_test_inner(b, b_2));                    \
            vec_free(&b);                                           \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_UTILS_BYTRE_VEC_INIT_IMMED(1, "0x", 0, ());
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(2, "0x00", 1, (0));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(3, "0x01", 1, (1));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(4, "0x0f", 1, (0xf));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(5, "0x10", 1, (0x10));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(6, "0x80", 1, (0x80));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(7, "0xf0", 1, (0xf0));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(8, "0xff", 1, (0xff));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(9, "0x0000", 2, (0, 0));
    TEST_UTILS_BYTRE_VEC_INIT_IMMED(10, "0x1234", 2, (0x12, 0x34));

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
            assert_byte(uint64_get_byte(0x0807060504030201, i), i + 1);
        }
        TEST_CASE_CLOSE
    }

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

void test_utils_uint64_init_byte(bool show)
{
    TEST_FN_OPEN

    #define TEST_UTILS_UINT64_INIT_BYTE(TAG, BYTES, RES)    \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            uint64_t res = uint64_init_byte_immed(BYTES);   \
            assert_64(res, RES);                            \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_UTILS_UINT64_INIT_BYTE(1, "0xff", 0xff);
    TEST_UTILS_UINT64_INIT_BYTE(2, "0xffee", 0xffee);
    TEST_UTILS_UINT64_INIT_BYTE(3, "0xffffffffffffffff", U64_MAX);
    TEST_UTILS_UINT64_INIT_BYTE(4, "0x0807060504030201", 0x0807060504030201);

    #undef TEST_UTILS_UINT64_INIT_BYTE

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



void tet_utils_vec_init_zero(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        byte_vec_t b = byte_vec_init_zero();
        assert(b.arr == NULL);
        assert(b.size == 0);
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
            assert(byte_vec_test_immed(b, RES));            \
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
    test_utils_byte_vec_init_immed(show);

    test_utils_uint64_add(show);
    test_utils_uint64_get_byte(show);
    test_utils_uint64_set_byte(show);
    test_utils_uint64_get_size(show);
    test_utils_uint64_init_byte(show);
    test_utils_uint128_to_uint64(show);

    tet_utils_vec_init_zero(show);
    test_utils_byte_vec_init_uint64(show);
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
