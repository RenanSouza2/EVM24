#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../../utils/clu/bin/header.h"
#include "../../../word/debug.h"



void test_stack_list_create()
{
    printf("\n\t%s\t\t", __func__);

    evm_word_t w = WORD(4, 3, 2, 1);
    stack_l_p sl = stack_l_create((stack_l_p)1, &w);
    assert(sl->sl == (stack_l_p)1);
    assert(word_immed(w, 4, 3, 2, 1));
    free(sl);

    sl = stack_l_create(NULL, &w);
    assert(sl->sl == NULL);
    free(sl);

    assert(mem_empty());
}

void test_stack_list_pop()
{
    printf("\n\t%s\t\t", __func__);

    evm_word_t w = WORD(4, 3, 2, 1);
    stack_l_p sl = stack_l_create((stack_l_p)1, &w);
    sl = stack_l_pop(sl);
    assert(sl == (stack_l_p)1);

    assert(mem_empty());
}



void test_stack_list()
{
    printf("\n%s\t\t", __func__);

    test_stack_list_create();
    test_stack_list_pop();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_stack_list();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
