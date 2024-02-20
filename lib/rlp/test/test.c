#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"


void test_rlp_decode()
{
    printf("\n\t%s\t\t", __func__);

    byte_vec_t b = byte_vec_init_immed("0xb9ffff");
    rlp_decode(b.v);

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
