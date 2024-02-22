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

    // evm_rlp_t r = rlp_decode_immed("0xf86780862d79883d2000825208945df9b87991262f6ba471f09758cde1c0fc1de734827a69801ca088ff6cf0fefd94db46111149ae4bfc179e9b94721fffd821d38d16464b3f71d0a045e0aff800961cfce805daef7016b9b675c137a6a41a548f7b60a3484c06a33a");
    // rlp_display(r);

    // f8 67 -> list length 103
    // 80
    // 86 2d79883d2000
    // 82 5208
    // 94 5df9b87991262f6ba471f09758cde1c0fc1de734
    // 82 7a69
    // 80
    // 1c 
    // a0 88ff6cf0fefd94db46111149ae4bfc179e9b94721fffd821d38d16464b3f71d0
    // a0 45e0aff800961cfce805daef7016b9b675c137a6a41a548f7b60a3484c06a33a

    // evm_rlp_t t = rlp_init_immed(LIST, 9,
    //     BYTE, "0x",
    //     BYTE, "0x2d79883d2000",
    //     BYTE, "0x5208",
    //     BYTE, "0x5df9b87991262f6ba471f09758cde1c0fc1de734",
    //     BYTE, "0x7a69",
    //     BYTE, "0x",
    //     BYTE, "0x1c",
    //     BYTE, "0x88ff6cf0fefd94db46111149ae4bfc179e9b94721fffd821d38d16464b3f71d0",
    //     BYTE, "0x45e0aff800961cfce805daef7016b9b675c137a6a41a548f7b60a3484c06a33a"
    // );
    // rlp_display(t);

    // malloc(1);
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