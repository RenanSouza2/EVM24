#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"



void test_node()
{
    TEST_LIB

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_node();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
