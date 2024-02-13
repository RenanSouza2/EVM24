#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

void test_vector()
{
    printf("\n%s\t\t", __func__);

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_vector();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
