#include <stdio.h>

#include "../debug.h"
#include "../../../../../testrc.h"
#include "../../../../../mods/macros/test.h"

#include "../../../../word/debug.h"
#include "../../../../utils/debug.h"



void test_stack_init(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        evm_stack_t s = stack_init();
        assert(stack_immed(s, 0) == true);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_stack_push(bool show)
{
    TEST_FN_OPEN

    #define TEST_STACK_PUSH(TAG, STACK_BEF, WORD, STACK_AFT)        \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            evm_stack_t s = stack_init_immed(ARG_OPEN STACK_BEF);   \
            word_t w1 = WORD;                                       \
            uint64_t res = stack_push(&s, &w1);                     \
            assert_64(res, 0);                                      \
            assert(stack_immed(s, ARG_OPEN STACK_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_STACK_PUSH(1,
        (0),
        W4(4, 3, 2, 1),
        (1, W4(4, 3, 2, 1))
    );

    TEST_STACK_PUSH(1,
        (1, W4(4, 3, 2, 1)),
        W4(1, 2, 3, 4),
        (2, W4(4, 3, 2, 1), W4(1, 2, 3, 4))
    );

    #undef TEST_STACK_PUSH
    
    TEST_CASE_OPEN(3)
    {
        evm_stack_t s = stack_init();
        s.count = 1024;
        word_t w = W4(0, 0, 0, 0);
        uint64_t res = stack_push(&s, &w);
        assert_64(res, 1);
        stack_free(&s);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_stack_pop(bool show)
{
    TEST_FN_OPEN

    #define TEST_STACK_POP(TAG, STACK_BEF, WORD, STACK_AFT)         \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            word_t w;                                               \
            evm_stack_t s = stack_init_immed(ARG_OPEN STACK_BEF);   \
            uint64_t res = stack_pop(&w, &s);                       \
            assert_64(res, 0);                                      \
            assert(word_test(w, WORD));                             \
            assert(stack_immed(s, ARG_OPEN STACK_AFT));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_STACK_POP(1,
        (2, W4(4, 3, 2, 1), W4(1, 2, 3, 4)),
        W4(1, 2, 3, 4),
        (1, W4(4, 3, 2, 1))
    );
    
    TEST_STACK_POP(2,
        (1, W4(4, 3, 2, 1)),
        W4(4, 3, 2, 1),
        (0)
    );

    TEST_CASE_OPEN(3)
    {
        word_t w;
        evm_stack_t s = stack_init_immed(0);
        uint64_t res = stack_pop(&w, &s);
        assert_64(res, 1);
        assert(stack_immed(s, 0));
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}




void test_stack()
{
    TEST_LIB

    bool show = true;

    test_stack_init(show);
    test_stack_push(show);
    test_stack_pop(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_stack();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
