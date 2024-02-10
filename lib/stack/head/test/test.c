#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../../utils/clu/bin/header.h"
#include "../../../word/debug.h"



void test_stack_init()
{
    printf("\n\t%s\t\t", __func__);

    evm_stack_t s = stack_init();
    assert(stack_test_immed(s, 0) == true);

    assert(mem_empty());
}

void test_stack_evm_push()
{
    printf("\n\t%s\t\t", __func__);

    printf("\n\t\t%s 1\t\t", __func__);
    evm_word_t w1 = WORD(4, 3, 2, 1);
    evm_stack_t s = stack_init();
    assert(stack_push(&s, &w1) == true);
    assert(stack_test_immed(s, 1, w1));

    printf("\n\t\t%s 2\t\t", __func__);
    evm_word_t w2 = WORD(1, 2, 3, 4);
    assert(stack_push(&s, &w2) == true);
    assert(stack_test_immed(s, 2, w2, w1));
    stack_free(&s);

    printf("\n\t\t%s 3\t\t", __func__);
    s = stack_init();
    evm_word_t w = WORD(0, 0, 0, 0);
    for(int i=0; i<1024; i++)
        assert(stack_push(&s, &w) == true);
    assert(stack_push(&s, &w) == false);
    stack_free(&s);

    assert(mem_empty());
}

void test_stack_evm_pop()
{
    printf("\n\t%s\t\t", __func__);

    evm_word_t w = WORD(0, 0, 0, 0);
    evm_stack_t s = stack_init();

    assert(stack_evm_push_immed(&s, WORD(4, 3, 2, 1)) == true);
    assert(stack_evm_push_immed(&s, WORD(1, 2, 3, 4)) == true);

    
    assert(stack_pop(&w, &s) == true);
    assert(word_test_immed(w, 1, 2, 3, 4) == true);
    assert(stack_test_immed(s, 1, WORD(4, 3, 2, 1)) == true);
    
    assert(stack_pop(&w, &s) == true);
    assert(word_test_immed(w, 4, 3, 2, 1) == true);
    assert(stack_test_immed(s, 0) == true);

    assert(stack_pop(&w, &s) == false);

    assert(mem_empty());
}




void test_stack()
{
    printf("\n%s\t\t", __func__);

    test_stack_init();
    test_stack_evm_push();
    test_stack_evm_pop();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_stack();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
