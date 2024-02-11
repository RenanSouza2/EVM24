#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../utils/debug.h"



void test_gas_mem()
{
    printf("\n%s\t\t", __func__);

    assert_64(gas_mem( 0),  0);
    assert_64(gas_mem( 1),  3);
    assert_64(gas_mem(22), 66);
    assert_64(gas_mem(23), 70);
    assert_64(gas_mem(32), 98);
    assert_64(gas_mem(U64_MAX), U64_MAX);

    assert(mem_empty());
}



void test_gas()
{
    printf("\n%s\t\t", __func__);

    test_gas_mem();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_gas();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
