#include "../debug.h"
#include "../../../../utils/assert.h"



void test_database()
{
    printf("\n%s\t\t", __func__);

    assert(clu_mem_is_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_database();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
