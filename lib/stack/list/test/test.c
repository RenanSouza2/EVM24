#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../../utils/clu/bin/header.h"
#include "../../../word/debug.h"

void test_stack_list_create()
{
    printf("\n\t%s\t\t", __func__);

    word_t w = WORD(4, 3, 2, 1);
    s_list_p sl = s_list_create((s_list_p)1, w);

    assert(mem_empty());
}


void test_stack_list()
{
    printf("\n%s\t\t", __func__);

    test_stack_list_create();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_stack_list();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
