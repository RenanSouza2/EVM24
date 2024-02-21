#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"


void test_rlp_decode()
{
    printf("\n\t%s\t\t", __func__);

    evm_rlp_t r = rlp_decode_immed("0x7f");
    rlp_display(r);

    assert(clu_mem_empty());
}



void test_rlp()
{
    printf("\n%s\t\t", __func__);

    test_rlp_decode();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_rlp();
    printf("\n\n\tTest successful\n\n");
    return 0;
}