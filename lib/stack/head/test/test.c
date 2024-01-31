#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../../utils/clu/bin/header.h"
#include "../../../word/debug.h"



void test_stack_head_initialize()
{
    printf("\n\t%s\t\t", __func__);

    stack_t s = stack_initialize();
    assert(s.sl == NULL);
    assert(s.count == 0);

    assert(mem_empty());
}


void test_stack_head_push()
{
    printf("\n\t%s\t\t", __func__);

    printf("\n\t\t%s 1\t\t", __func__);
    word_t w1 = WORD(4, 3, 2, 1);
    stack_t s = stack_initialize();
    assert(stack_push(&s, &w1) == true);
    assert(stack(s, 1, w1));

    printf("\n\t\t%s 2\t\t", __func__);
    word_t w2 = WORD(1, 2, 3, 4);
    assert(stack_push(&s, &w2) == true);
    assert(stack(s, 2, w2, w1));

    stack_free(&s);

    assert(mem_empty());
}




void test_stack_head()
{
    printf("\n%s\t\t", __func__);

    test_stack_head_initialize();
    test_stack_head_push();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_stack_head();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
