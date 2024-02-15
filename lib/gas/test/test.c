#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../utils/debug.h"



void test_gas_mem()
{
    printf("\n\t%s\t\t", __func__);

    assert_64(gas_mem( 0),  0);
    assert_64(gas_mem( 1),  3);
    assert_64(gas_mem(22), 66);
    assert_64(gas_mem(23), 70);
    assert_64(gas_mem(32), 98);
    assert_64(gas_mem(0xffffffffff), U64_MAX);
    assert_64(gas_mem(     U64_MAX), U64_MAX);

    assert(mem_empty());
}

void test_gas_copy()
{
    printf("\n\t%s\t\t", __func__);

    assert_64(gas_copy( 0), 3);
    assert_64(gas_copy( 1), 6);
    assert_64(gas_copy(32), 6);
    assert_64(gas_copy(64), 9);
    assert_64(gas_copy(U64_MAX), 0x1800000000000003);

    assert(mem_empty());
}



void test_gas()
{
    printf("\n%s\t\t", __func__);

    test_gas_mem();
    test_gas_copy();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_gas();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
