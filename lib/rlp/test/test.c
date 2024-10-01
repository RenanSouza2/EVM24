#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

#include "../../utils/debug.h"



void test_rlp_encode(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    byte_vec_t b = rlp_encode_immed(BYTES, "0x");
    assert(byte_vec_test_immed(b, "0x80"));
    vec_free(&b);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x00");
    assert(byte_vec_test_immed(b, "0x00"));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s  3\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x01");
    assert(byte_vec_test_immed(b, "0x01"));
    vec_free(&b);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x7f");
    assert(byte_vec_test_immed(b, "0x7f"));
    vec_free(&b);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x80");
    assert(byte_vec_test_immed(b, "0x8180"));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s  6\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x80");
    assert(byte_vec_test_immed(b, "0x8180"));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s  7\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0xff");
    assert(byte_vec_test_immed(b, "0x81ff"));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s  8\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x0000");
    assert(byte_vec_test_immed(b, "0x820000"));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s  9\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    );
    assert(byte_vec_test_immed(b, "0xb7" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s 10\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "3031323334353637"
    );
    assert(byte_vec_test_immed(b, "0xb838" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "3031323334353637"
    ));
    vec_free(&b);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    );
    assert(byte_vec_test_immed(b, "0xb8ff"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    ));
    vec_free(&b);

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    b = rlp_encode_immed(BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    );
    assert(byte_vec_test_immed(b, "0xb90100"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    ));
    vec_free(&b);

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    b = rlp_encode_immed(LIST, 0);
    assert(byte_vec_test_immed(b, "0xc0"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x00");
    assert(byte_vec_test_immed(b, "0xc100"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x80");
    assert(byte_vec_test_immed(b, "0xc28180"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, BYTES, "0x00", BYTES, "0x01");
    assert(byte_vec_test_immed(b, "0xc20001"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, LIST, 0, BYTES, "0x01");
    assert(byte_vec_test_immed(b, "0xc2c001"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 18\t\t", __func__);
    b = rlp_encode_immed(LIST, 2, LIST, 1, BYTES, "0x00", BYTES, "0x01");
    assert(byte_vec_test_immed(b, "0xc3c10001"));
    vec_free(&b);

    if(show) printf("\n\t\t%s 19\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    );
    assert(byte_vec_test_immed(b, "0xf7b6"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    ));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s 20\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    );
    assert(byte_vec_test_immed(b, "0xf838b7"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ));
    vec_free(&b);
    
    if(show) printf("\n\t\t%s 21\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfc"
    );
    assert(byte_vec_test_immed(b, "0xf8ffb8fd"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfc"
    ));
    vec_free(&b);

    if(show) printf("\n\t\t%s 22\t\t", __func__);
    b = rlp_encode_immed(LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfd"
    );
    assert(byte_vec_test_immed(b, "0xf90100b8fe"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfd"
    ));
    vec_free(&b);

    assert(clu_mem_empty());
}

void test_rlp_decode(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    evm_rlp_t r;
    assert_64(rlp_decode_immed(&r, "0x80"), 0);
    assert(rlp_test_immed(r, BYTES, "0x"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x00"), 0);
    assert(rlp_test_immed(r, BYTES, "0x00"));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s  3\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x01"), 0);
    assert(rlp_test_immed(r, BYTES, "0x01"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x7f"), 0);
    assert(rlp_test_immed(r, BYTES, "0x7f"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x8180"), 0);
    assert(rlp_test_immed(r, BYTES, "0x80"));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s  6\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x8180"), 0);
    assert(rlp_test_immed(r, BYTES, "0x80"));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s  7\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x81ff"), 0);
    assert(rlp_test_immed(r, BYTES, "0xff"));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s  8\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x820000"), 0);
    assert(rlp_test_immed(r, BYTES, "0x0000"));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s  9\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb7" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ), 0);
    assert(rlp_test_immed(r, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s 10\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb838" 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "3031323334353637"
    ), 0);
    assert(rlp_test_immed(r, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "3031323334353637"
    ));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb8ff"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    ), 0);
    assert(rlp_test_immed(r, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    ));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb90100"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    ), 0);
    assert(rlp_test_immed(r, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
	));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc0"), 0);
    assert(rlp_test_immed(r, LIST, 0));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc100"), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x00"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc28180"), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x80"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc20001"), 0);
    assert(rlp_test_immed(r, LIST, 2, BYTES, "0x00", BYTES, "0x01"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc2c001"), 0);
    assert(rlp_test_immed(r, LIST, 2, LIST, 0, BYTES, "0x01"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 18\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc3c10001"), 0);
    assert(rlp_test_immed(r, LIST, 2, LIST, 1, BYTES, "0x00", BYTES, "0x01"));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 19\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf7b6"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    ), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435"
    ));
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s 20\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf838b7"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "30313233343536"
    ));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 21\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf8ffb8fd"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfc"
    ), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfc"
    ));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 22\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf90100b8fe"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfd"
    ), 0);
    assert(rlp_test_immed(r, LIST, 1, BYTES, "0x"
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfd"
    ));
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 23\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x"), 0x11);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 24\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x81"), 0x21);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 25\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x8100"), 0x31);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 26\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb8"), 0x141);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 27\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb801"), 0x241);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 28\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xb838"), 0x61);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 29\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc1"), 0x61);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 30\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf8"), 0x151);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 31\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf801"), 0x251);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 32\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xf838"), 0x61);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 33\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc181"), 0x21113);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 34\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc20081"),  0x21113);
    assert(clu_mem_empty());
    
    if(show) printf("\n\t\t%s 35\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc1c10081"), 0x2);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 36\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0xc1c10081"), 0x2);
    assert(clu_mem_empty());

    if(show) printf("\n\t\t%s 37\t\t", __func__);
    assert_64(rlp_decode_immed(&r, "0x0000"), 0x2);
    assert(clu_mem_empty());
}



void test_rlp()
{
    printf("\n%s\t\t", __func__);

    test_rlp_encode(false);
    test_rlp_decode(true);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_rlp();
    printf("\n\n\tTest successful\n\n");
    return 0;
}