#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../../utils/debug.h"



void test_gas_mem(bool show)
{
    TEST_FN_OPEN

    #define TEST_GAS_MEM(TAG, SIZE, RES)    \
    {                                       \
        TEST_CASE_OPEN(TAG)                 \
        {                                   \
            uint64_t res = gas_mem(SIZE);   \
            assert_64(res, RES);            \
        }                                   \
        TEST_CASE_CLOSE                     \
    }

    TEST_GAS_MEM(1,  0,  0);
    TEST_GAS_MEM(2,  1,  3);
    TEST_GAS_MEM(3, 22, 66);
    TEST_GAS_MEM(4, 23, 70);
    TEST_GAS_MEM(5, 32, 98);
    TEST_GAS_MEM(6, 0xffffffffff, U64_MAX);
    TEST_GAS_MEM(7, U64_MAX, U64_MAX);

    #undef TEST_GAS_MEM

    TEST_FN_CLOSE
}

void test_gas_copy(bool show)
{
    TEST_FN_OPEN

    #define TEST_GAS_COPY(TAG, SIZE, RES)   \
    {                                       \
        TEST_CASE_OPEN(TAG)                 \
        {                                   \
            uint64_t res = gas_copy(SIZE);  \
            assert_64(res, RES);            \
        }                                   \
        TEST_CASE_CLOSE                     \
    }

    TEST_GAS_COPY(1,  0, 3);
    TEST_GAS_COPY(2,  1, 6);
    TEST_GAS_COPY(3, 32, 6);
    TEST_GAS_COPY(4, 64, 9);
    TEST_GAS_COPY(5, U64_MAX, 0x1800000000000003);

    TEST_FN_CLOSE
}



void test_gas()
{
    TEST_LIB

    bool show = true;

    test_gas_mem(show);
    test_gas_copy(show);

    TEST_ASSERT_MEM_EMPTY
}


int main()
{
    setbuf(stdout, NULL);
    test_gas();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
