#include "../debug.h"
#include "../../../utils/clu/bin/header.h"


void test_utils_uint64_t_display()
{
    printf("\n%s\t\t", __func__);

    // uint64_t_display(1);

    assert(mem_empty());
}

void test_utils()
{
    printf("\n%s\t\t", __func__);

    test_utils_uint64_t_display();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
