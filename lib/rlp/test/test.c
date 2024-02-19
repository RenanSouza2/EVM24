#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_rlp()
{
    printf("\n%s\t\t", __func__);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_rlp();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
