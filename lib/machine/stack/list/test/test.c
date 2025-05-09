#include "../debug.h"
#include "../../../../../testrc.h"
#include "../../../../../mods/macros/test.h"

#include "../../../../word/debug.h"



void test_stack_list_create(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        word_t w = W4(4, 3, 2, 1);
        evm_stack_l_p sl = stack_l_create((evm_stack_l_p)1, &w);
        assert(sl->sl == (evm_stack_l_p)1);
        assert(word_test(w, W4(4, 3, 2, 1)));
        free(sl);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        word_t w = W4(4, 3, 2, 1);
        evm_stack_l_p sl = stack_l_create(NULL, &w);
        assert(sl->sl == NULL);
        free(sl);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_stack_list_pop(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        word_t w = W4(4, 3, 2, 1);
        evm_stack_l_p sl = stack_l_create((evm_stack_l_p)1, &w);
        sl = stack_l_pop(NULL, sl);
        assert(sl == (evm_stack_l_p)1);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        word_t w_out;
        word_t w = W4(4, 3, 2, 1);
        evm_stack_l_p sl = stack_l_create((evm_stack_l_p)1, &w);
        sl = stack_l_pop(&w_out, sl);
        assert(word_test(w_out, W4(4, 3, 2, 1)));
        assert(sl == (evm_stack_l_p)1);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_stack_list()
{
    TEST_LIB

    bool show = false;

    test_stack_list_create(show);
    test_stack_list_pop(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_stack_list();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
