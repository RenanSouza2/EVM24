#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../utils/debug.h"


#define REPEAT(HEAD)                                                                \
    #HEAD "0" #HEAD "1" #HEAD "2" #HEAD "3" #HEAD "4" #HEAD "5" #HEAD "6" #HEAD "7" \
    #HEAD "8" #HEAD "9" #HEAD "a" #HEAD "b" #HEAD "c" #HEAD "d" #HEAD "e" #HEAD "f"
#define F00T2F REPEAT(0) REPEAT(1) REPEAT(2)
#define F00TEF                                          \
    REPEAT(0) REPEAT(1) REPEAT(2) REPEAT(3) REPEAT(4)   \
    REPEAT(5) REPEAT(6) REPEAT(7) REPEAT(8) REPEAT(9)   \
    REPEAT(a) REPEAT(b) REPEAT(c) REPEAT(d) REPEAT(e)



void test_rlp_encode(bool show)
{
    TEST_FN_OPEN

    #define TEST_RLP_ENCODE(TAG, VALUES, RES)               \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            evm_rlp_t r = rlp_init_immed(ARG_OPEN VALUES);  \
            byte_vec_t b = rlp_encode(&r);                  \
            assert(byte_vec_immed(b, RES));                 \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_RLP_ENCODE(1, (BYTES, "0x"), "0x80");
    TEST_RLP_ENCODE(2, (BYTES, "0x00"), "0x00");
    TEST_RLP_ENCODE(3, (BYTES, "0x01"), "0x01");
    TEST_RLP_ENCODE(4, (BYTES, "0x7f"), "0x7f");
    TEST_RLP_ENCODE(5, (BYTES, "0x80"), "0x8180");
    TEST_RLP_ENCODE(6, (BYTES, "0x81"), "0x8181");
    TEST_RLP_ENCODE(7, (BYTES, "0xff"), "0x81ff");
    TEST_RLP_ENCODE(8, (BYTES, "0x0000"), "0x820000");
    TEST_RLP_ENCODE(9,
        (BYTES, "0x" F00T2F "30313233343536"),
        "0xb7" F00T2F "30313233343536"
    );
    TEST_RLP_ENCODE(10,
        (BYTES, "0x" F00T2F "3031323334353637"),
        "0xb838" F00T2F "3031323334353637"
    );
    TEST_RLP_ENCODE(11,
        (BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe"),
        "0xb8ff" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe"
    );
    TEST_RLP_ENCODE(12,
        (BYTES, "0x" F00TEF REPEAT(f)),
        "0xb90100" F00TEF REPEAT(f)
    );
    TEST_RLP_ENCODE(13, (LIST, 0), "0xc0");
    TEST_RLP_ENCODE(14, (LIST, 1, BYTES, "0x00"), "0xc100");
    TEST_RLP_ENCODE(15, (LIST, 1, BYTES, "0x80"), "0xc28180");
    TEST_RLP_ENCODE(16, (LIST, 2, BYTES, "0x00", BYTES, "0x01"), "0xc20001");
    TEST_RLP_ENCODE(17, (LIST, 2, LIST, 0, BYTES, "0x01"), "0xc2c001");
    TEST_RLP_ENCODE(18, (LIST, 2, LIST, 1, BYTES, "0x00", BYTES, "0x01"), "0xc3c10001");
    TEST_RLP_ENCODE(19,
        (LIST, 1, BYTES, "0x" F00T2F "303132333435"),
        "0xf7b6" F00T2F "303132333435"
    );
    TEST_RLP_ENCODE(20,
        (LIST, 1, BYTES, "0x" F00T2F "30313233343536"),
        "0xf838b7" F00T2F "30313233343536"
    );
    TEST_RLP_ENCODE(21,
        (LIST, 1, BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfc"),
        "0xf8ffb8fd" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfc"
    );
    TEST_RLP_ENCODE(22,
        (LIST, 1, BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfd"),
        "0xf90100b8fe" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfd"
    );
    TEST_FN_CLOSE
}

void test_rlp_decode(bool show)
{
    TEST_FN_OPEN

    #define TEST_RLP_DECODE(TAG, BYTES_IN, RES)             \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            evm_rlp_t r;                                    \
            byte_vec_t b = byte_vec_init_immed(BYTES_IN);   \
            uint64_t err = rlp_decode(&r, &b);              \
            assert_64(err, 0);                              \
            assert(rlp_immed(r, ARG_OPEN RES));             \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_RLP_DECODE(1, "0x80", (BYTES, "0x"));
    TEST_RLP_DECODE(2, "0x00", (BYTES, "0x00"));
    TEST_RLP_DECODE(3, "0x01", (BYTES, "0x01"));
    TEST_RLP_DECODE(4, "0x7f", (BYTES, "0x7f"));
    TEST_RLP_DECODE(5, "0x8180", (BYTES, "0x80"));
    TEST_RLP_DECODE(6, "0x8180", (BYTES, "0x80"));
    TEST_RLP_DECODE(7, "0x81ff", (BYTES, "0xff"));
    TEST_RLP_DECODE(8, "0x820000", (BYTES, "0x0000"));
    TEST_RLP_DECODE(9,
        "0xb7" F00T2F "30313233343536",
        (BYTES, "0x" F00T2F "30313233343536")
    );
    TEST_RLP_DECODE(10,
        "0xb838" F00T2F "3031323334353637",
        (BYTES, "0x" F00T2F "3031323334353637")
    );
    TEST_RLP_DECODE(11,
        "0xb8ff" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe",
        (BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe")
    );
    TEST_RLP_DECODE(12, "0xb90100" F00TEF REPEAT(f), (BYTES, "0x" F00TEF REPEAT(f)));
    TEST_RLP_DECODE(13, "0xc0", (LIST, 0));
    TEST_RLP_DECODE(14, "0xc100", (LIST, 1, BYTES, "0x00"));
    TEST_RLP_DECODE(15, "0xc28180", (LIST, 1, BYTES, "0x80"));
    TEST_RLP_DECODE(16, "0xc20001", (LIST, 2, BYTES, "0x00", BYTES, "0x01"));
    TEST_RLP_DECODE(17, "0xc2c001", (LIST, 2, LIST, 0, BYTES, "0x01") );
    TEST_RLP_DECODE(18, "0xc3c10001", (LIST, 2, LIST, 1, BYTES, "0x00", BYTES, "0x01"));
    TEST_RLP_DECODE(19,
        "0xf7b6" F00T2F "303132333435",
        (LIST, 1, BYTES, "0x" F00T2F "303132333435")
    );
    TEST_RLP_DECODE(20,
        "0xf838b7" F00T2F "30313233343536",
        (LIST, 1, BYTES, "0x" F00T2F "30313233343536")
    );
    TEST_RLP_DECODE(21,
        "0xf8ffb8fd" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfc",
        (LIST, 1, BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfc")
    );
    TEST_RLP_DECODE(22,
        "0xf90100b8fe" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfd",
        (LIST, 1, BYTES, "0x" F00TEF "f0f1f2f3f4f5f6f7f8f9fafbfcfd")
    );

    #undef TEST_RLP_DECODE

    #define TEST_RLP_DECODE(TAG, BYTES_IN, ERR)             \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            evm_rlp_t r;                                    \
            byte_vec_t b = byte_vec_init_immed(BYTES_IN);   \
            uint64_t err = rlp_decode(&r, &b);              \
            assert_64(err, ERR);                            \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_RLP_DECODE(23, "0x", 0x11);
    TEST_RLP_DECODE(24, "0x81", 0x21);
    TEST_RLP_DECODE(25, "0x8100", 0x31);
    TEST_RLP_DECODE(26, "0xb8", 0x141);
    TEST_RLP_DECODE(27, "0xb90001", 0x241);
    TEST_RLP_DECODE(28, "0xb801", 0x341);
    TEST_RLP_DECODE(29, "0xb838", 0x61);
    TEST_RLP_DECODE(30, "0xc1", 0x61);
    TEST_RLP_DECODE(31, "0xf8", 0x151);
    TEST_RLP_DECODE(32, "0xf90001", 0x251);
    TEST_RLP_DECODE(33, "0xf801", 0x351);
    TEST_RLP_DECODE(34, "0xf838", 0x61);
    TEST_RLP_DECODE(35, "0xc181", 0x21113);
    TEST_RLP_DECODE(36, "0xc20081", 0x21113);
    TEST_RLP_DECODE(37, "0xc1c10081", 0x2);
    TEST_RLP_DECODE(38, "0xc1c10081", 0x2);
    TEST_RLP_DECODE(39, "0x0000", 0x2);

    #undef TEST_RLP_DECODE

    TEST_FN_CLOSE
}



void test_rlp()
{
    TEST_LIB

    bool show = false;

    test_rlp_encode(show);
    test_rlp_decode(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_rlp();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
