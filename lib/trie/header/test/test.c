
#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"



void test_rlp_header()
{
    TEST_LIB

    // bool show = true;

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_rlp_header();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
