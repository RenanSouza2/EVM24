#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

void test_word_size()
{
    printf("\n\t%s\t\t", __func__);
    
    assert(sizeof(word_t) == 32);
}


void test_word()
{
    printf("\n%s\t\t", __func__);

    test_word_size();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_word();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
