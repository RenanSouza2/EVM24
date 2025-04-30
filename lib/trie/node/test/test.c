#include "../debug.h"
#include "../../../../utils/assert.h"

void test_node()
{
    printf("\n%s\t\t", __func__);

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_node();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
