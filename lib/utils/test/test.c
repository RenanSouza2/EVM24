#include "../debug.h"
#include "../../../utils/clu/bin/header.h"



void test_utils_uint64_add()
{
    printf("\n\t%s", __func__);

    assert_64(uint64_add(1, 2), 3);
    assert_64(uint64_add(UINT64_MAX, 2), UINT64_MAX);

    assert(mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

    test_utils_uint64_add();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
