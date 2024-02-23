#include <stdio.h>

#include "../debug.h"
#include "../../../../../utils/clu/bin/header.h"

#include "../../../word/debug.h"
#include "../../../../utils/debug.h"



void test_stack_list_create()
{
    printf("\n\t%s", __func__);

    evm_word_t w = WORD(4, 3, 2, 1);
    evm_stack_l_p sl = stack_l_create((evm_stack_l_p)1, &w);
    assert(sl->sl == (evm_stack_l_p)1);
    assert(word_test(w, WORD(4, 3, 2, 1)));
    free(sl);

    sl = stack_l_create(NULL, &w);
    assert(sl->sl == NULL);
    free(sl);

    assert(clu_mem_empty());
}

void test_stack_list_pop()
{
    printf("\n\t%s", __func__);

    evm_word_t w = WORD(4, 3, 2, 1);
    evm_stack_l_p sl = stack_l_create((evm_stack_l_p)1, &w);
    sl = stack_l_pop(NULL, sl);
    assert(sl == (evm_stack_l_p)1);

    evm_word_t w_out;
    w = WORD(4, 3, 2, 1);
    sl = stack_l_create((evm_stack_l_p)1, &w);
    sl = stack_l_pop(&w_out, sl);
    assert(word_test(w_out, WORD(4, 3, 2, 1)));
    assert(sl == (evm_stack_l_p)1);

    assert(clu_mem_empty());
}



void test_stack_list()
{
    printf("\n%s", __func__);

    test_stack_list_create();
    test_stack_list_pop();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_stack_list();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
