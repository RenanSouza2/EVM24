#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

void test_hello()
{
    printf("\n\t%s\t\t", __func__);

    hello_debug();
    hello_example();
}

void test_malloc()
{
    printf("\n\t%s\t\t", __func__);

    handler_p h = hello_malloc();

    // Uncomment line under to see assertion fail
    // assert(mem_empty());

    printf("\nThis prints the current allocated pointers");
    mem_report("TAG");

    free(h);

    assert(mem_empty());
}

void test_example()
{
    printf("\n%s\t\t", __func__);

    test_hello();
    test_malloc();
}


int main() 
{
    setbuf(stdout, NULL);
    test_example();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
