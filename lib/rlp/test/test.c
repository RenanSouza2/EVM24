#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"


void test_rlp_encode()
{
    printf("\n\t%s\t\t", __func__);

    // printf("\n\t\t%s  1\t\t", __func__);
    byte_vec_t b = rlp_encode_immed(BYTE, "0x");
    assert(byte_vec_test_immed(b, "0x80"));
    byte_vec_free(&b);

    // printf("\n\t\t%s  2\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x00");
    assert(byte_vec_test_immed(b, "0x00"));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s  3\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x01");
    assert(byte_vec_test_immed(b, "0x01"));
    byte_vec_free(&b);

    // printf("\n\t\t%s  4\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x7f");
    assert(byte_vec_test_immed(b, "0x7f"));
    byte_vec_free(&b);

    // printf("\n\t\t%s  5\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x80");
    assert(byte_vec_test_immed(b, "0x8180"));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s  6\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x80");
    assert(byte_vec_test_immed(b, "0x8180"));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s  7\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0xff");
    assert(byte_vec_test_immed(b, "0x81ff"));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s  8\t\t", __func__);
    b = rlp_encode_immed(BYTE, "0x0000");
    assert(byte_vec_test_immed(b, "0x820000"));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s  9\t\t", __func__);
    b = rlp_encode_immed(BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    );
    assert(byte_vec_test_immed(b, 
        "0xb7" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f3"
        "0313233343536"
    ));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s 10\t\t", __func__);
    b = rlp_encode_immed(BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "3031323334353637"
    );
    assert(byte_vec_test_immed(b, 
        "0xb838" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f3"
        "031323334353637"
    ));
    byte_vec_free(&b);

    // printf("\n\t\t%s 11\t\t", __func__);
    b = rlp_encode_immed(BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    );
    assert(byte_vec_test_immed(b, 
        "0xb8ff"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    ));
    byte_vec_free(&b);

    // printf("\n\t\t%s 12\t\t", __func__);
    b = rlp_encode_immed(BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    );
    assert(byte_vec_test_immed(b, 
        "0xb90100"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    ));
    byte_vec_free(&b);

    // printf("\n\t\t%s 13\t\t", __func__);
    b = rlp_encode_immed(LIST, 0);
    assert(byte_vec_test_immed(b, "0xc0"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 14\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, "0x00");
    assert(byte_vec_test_immed(b, "0xc100"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 15\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, "0x80");
    assert(byte_vec_test_immed(b, "0xc28180"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 16\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, "0x80");
    assert(byte_vec_test_immed(b, "0xc28180"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 17\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, "0x80");
    assert(byte_vec_test_immed(b, "0xc28180"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 18\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, BYTE, "0x00", BYTE, "0x01");
    assert(byte_vec_test_immed(b, "0xc20001"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 18\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, LIST, 0, BYTE, "0x01");
    assert(byte_vec_test_immed(b, "0xc2c001"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 18\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, LIST, 1, BYTE, "0x00", BYTE, "0x01");
    assert(byte_vec_test_immed(b, "0xc3c10001"));
    byte_vec_free(&b);

    // printf("\n\t\t%s 19\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    );
    assert(byte_vec_test_immed(b,
        "0xf7b6"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    ));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s 20\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    );
    assert(byte_vec_test_immed(b,
        "0xf838b7"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ));
    byte_vec_free(&b);
    
    // printf("\n\t\t%s 21\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfc"
    );
    assert(byte_vec_test_immed(b,
        "0xf8ffb8fd"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfc"
    ));
    byte_vec_free(&b);

    // printf("\n\t\t%s 22\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTE, 
      "0x000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfd"
    );
    assert(byte_vec_test_immed(b,
        "0xf90100b8fe"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfd"
    ));
    byte_vec_free(&b);

    assert(clu_mem_empty());
}

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

    test_rlp_encode();
    // test_rlp_decode();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_rlp();
    printf("\n\n\tTest successful\n\n");
    return 0;
}