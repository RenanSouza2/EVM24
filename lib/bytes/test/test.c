#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

void test_bytes()
{
    printf("\n%s\t\t", __func__);

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
