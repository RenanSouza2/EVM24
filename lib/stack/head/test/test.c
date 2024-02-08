#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../../utils/clu/bin/header.h"
#include "../../../word/debug.h"



void test_stack_init()
{
    printf("\n\t%s\t\t", __func__);

    stack_d_t s = stack_init();
    assert(stack_immed(s, 0) == true);

    assert(mem_empty());
}

void test_stack_d_push()
{
    printf("\n\t%s\t\t", __func__);

    printf("\n\t\t%s 1\t\t", __func__);
    word_t w1 = WORD(4, 3, 2, 1);
    stack_d_t s = stack_init();
    assert(stack_d_push(&s, &w1) == true);
    assert(stack_immed(s, 1, w1));

    printf("\n\t\t%s 2\t\t", __func__);
    word_t w2 = WORD(1, 2, 3, 4);
    assert(stack_d_push(&s, &w2) == true);
    assert(stack_immed(s, 2, w2, w1));
    stack_free(&s);

    printf("\n\t\t%s 3\t\t", __func__);
    s = stack_init();
    word_t w = WORD(0, 0, 0, 0);
    for(int i=0; i<1024; i++)
        assert(stack_d_push(&s, &w) == true);
    assert(stack_d_push(&s, &w) == false);
    stack_free(&s);

    assert(mem_empty());
}

void test_stack_d_pop()
{
    printf("\n\t%s\t\t", __func__);

    word_t w = WORD(0, 0, 0, 0);
    stack_d_t s = stack_init();

    assert(stack_d_push_immed(&s, WORD(4, 3, 2, 1)) == true);
    assert(stack_d_push_immed(&s, WORD(1, 2, 3, 4)) == true);

    
    assert(stack_d_pop(&w, &s) == true);
    assert(word_immed(w, 1, 2, 3, 4) == true);
    assert(stack_immed(s, 1, WORD(4, 3, 2, 1)) == true);
    
    assert(stack_d_pop(&w, &s) == true);
    assert(word_immed(w, 4, 3, 2, 1) == true);
    assert(stack_immed(s, 0) == true);

    assert(stack_d_pop(&w, &s) == false);



    assert(mem_empty());
}




void test_stack()
{
    printf("\n%s\t\t", __func__);

    test_stack_init();
    test_stack_d_push();
    test_stack_d_pop();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_stack();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
