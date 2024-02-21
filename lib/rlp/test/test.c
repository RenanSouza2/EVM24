#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"


void test_rlp_decode()
{
    printf("\n\t%s\t\t", __func__);

    // evm_rlp_t r = rlp_decode_immed("0x7f");
    // rlp_display(r);

    // evm_rlp_t r = rlp_decode_immed("0x80");
    // rlp_display(r);
    // printf("r.type: %llu", r.type);

    evm_rlp_t r = rlp_decode_immed("0xf86780862d79883d2000825208945df9b87991262f6ba471f09758cde1c0fc1de734827a69801ca088ff6cf0fefd94db46111149ae4bfc179e9b94721fffd821d38d16464b3f71d0a045e0aff800961cfce805daef7016b9b675c137a6a41a548f7b60a3484c06a33a");
    rlp_display(r);

    // 0xf8 67 -> list length 103
    // 0x80862d79883d2000825208945df9b87991262f6ba471f09758cde1c0fc1de734827a69801ca088ff6cf0fefd94db46111149ae4bfc179e9b94721fffd821d38d16464b3f71d0a045e0aff800961cfce805daef7016b9b675c137a6a41a548f7b60a3484c06a33a

    malloc(1);
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