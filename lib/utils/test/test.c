#include "../debug.h"
#include "../../../utils/clu/bin/header.h"



void test_utils_cton()
{
    printf("\n\t%s", __func__);

    char ascii[] = {
        '0', '1', '2', '3', 
        '4', '5', '6', '7', 
        '8', '9', 'a', 'b', 
        'c', 'd', 'e', 'f'
    };

    for(uint64_t i=0; i<16; i++)
        assert(cton(ascii[i]) == i);

    char ASCII[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(uint64_t i=0; i<6; i++)
        assert(cton(ASCII[i]) == i + 10);

    assert(clu_mem_empty());
}

void test_byte_vec_init_immed()
{
    printf("\n\t%s", __func__);

    // printf("\n\t\t%s  1", __func__);
    byte_vec_t b = byte_vec_init_immed("0x");
    assert(b.v == NULL);
    assert(b.size == 0);

    // printf("\n\t\t%s  2", __func__);
    b = byte_vec_init_immed("0x00");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 0);
    byte_vec_free(&b);

    // printf("\n\t\t%s  3", __func__);
    b = byte_vec_init_immed("0x01");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 1);
    byte_vec_free(&b);

    // printf("\n\t\t%s  4", __func__);
    b = byte_vec_init_immed("0x0f");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 15);
    byte_vec_free(&b);

    // printf("\n\t\t%s  5", __func__);
    b = byte_vec_init_immed("0x10");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 16);
    byte_vec_free(&b);

    // printf("\n\t\t%s  6", __func__);
    b = byte_vec_init_immed("0x80");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 128);
    byte_vec_free(&b);

    // printf("\n\t\t%s  7", __func__);
    b = byte_vec_init_immed("0xf0");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 240);
    byte_vec_free(&b);

    // printf("\n\t\t%s  8", __func__);
    b = byte_vec_init_immed("0xff");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 255);
    byte_vec_free(&b);

    // printf("\n\t\t%s  9", __func__);
    b = byte_vec_init_immed("0x0000");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 0);
    assert(b.v[1] == 0);
    byte_vec_free(&b);

    // printf("\n\t\t%s 10", __func__);
    b = byte_vec_init_immed("0x1234");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 18);
    assert(b.v[1] == 52);
    byte_vec_free(&b);

    assert(clu_mem_empty());
}



void test_utils_uint64_add()
{
    printf("\n\t%s", __func__);

    assert_64(uint64_add(1, 2), 3);
    assert_64(uint64_add(UINT64_MAX, 2), UINT64_MAX);

    assert(clu_mem_empty());
}

void test_utils_uint64_get_byte()
{
    printf("\n\t%s", __func__);

    for(uint64_t i=0; i<8; i++)
        assert_byte(uint64_get_byte(0x0807060504030201, i), i+1);

    assert(clu_mem_empty());
}


void test_utils_uint64_set_byte()
{
    printf("\n\t%s", __func__);

    assert_64(uint64_set_byte(   0, 0, 0xff),   0xff);
    assert_64(uint64_set_byte(0xff, 0, 0xee),   0xee);
    assert_64(uint64_set_byte(   0, 1, 0xff), 0xff00);
    assert_64(uint64_set_byte(0xff, 1, 0xee), 0xeeff);
    
    for(uint64_t i=0; i<8; i++)
        assert_64(uint64_set_byte(0, i, 0xff), (uint64_t)0xff << (i << 3));

    uint64_t u = 0;
    for(uint64_t i=0; i<8; i++)
        u = uint64_set_byte(u, i, i+1);
    assert_64(u, 0x0807060504030201);

    assert(clu_mem_empty());
}

void test_utils_uint64_get_size()
{
    printf("\n\t%s", __func__);

    assert_64(uint64_get_size(0), 0);
    assert_64(uint64_get_size(1), 1);
    assert_64(uint64_get_size(0xff), 1);
    assert_64(uint64_get_size(0x100), 2);
    assert_64(uint64_get_size(U64_FF), 8);
    assert_64(uint64_get_size(U64_MAX), 8);
    
    assert(clu_mem_empty());
}


void test_utils_uint64_init_byte()
{
    printf("\n\t%s", __func__);

    assert_64(uint64_init_byte_immed(  "0xff"),   0xff);
    assert_64(uint64_init_byte_immed("0xffee"), 0xffee);
    assert_64(uint64_init_byte_immed("0xffffffffffffffff"), U64_MAX);
    assert_64(uint64_init_byte_immed("0x0807060504030201"), 0x0807060504030201);

    assert(clu_mem_empty());
}

void test_utils_uint128_to_uint64()
{
    printf("\n\t%s", __func__);

    assert_64(uint128_to_uint64(0), 0);
    assert_64(uint128_to_uint64(1), 1);
    assert_64(uint128_to_uint64(U64_MAX - 1), U64_MAX - 1);
    assert_64(uint128_to_uint64(U64_MAX), U64_MAX);
    assert_64(uint128_to_uint64(U128_1), U64_MAX);
    assert_64(uint128_to_uint64(U128_MAX), U64_MAX);

    assert(clu_mem_empty());
}




void test_utils_byte_vec_init_uint64()
{
    printf("\n\t%s", __func__);

    byte_vec_t b = byte_vec_init_uint64(0);
    assert(byte_vec_test_immed(b, "0x"));
    byte_vec_free(&b);

    b = byte_vec_init_uint64(1);
    assert(byte_vec_test_immed(b, "0x01"));
    byte_vec_free(&b);

    b = byte_vec_init_uint64(0xff);
    assert(byte_vec_test_immed(b, "0xff"));
    byte_vec_free(&b);

    b = byte_vec_init_uint64(0x100);
    assert(byte_vec_test_immed(b, "0x0100"));
    byte_vec_free(&b);

    b = byte_vec_init_uint64(U64_FF);
    assert(byte_vec_test_immed(b, "0xff00000000000000"));
    byte_vec_free(&b);

    b = byte_vec_init_uint64(U64_MAX);
    assert(byte_vec_test_immed(b, "0xffffffffffffffff"));
    byte_vec_free(&b);

    
    assert(clu_mem_empty());
}

void test_utils_uint64_vec_has_uint64()
{
    printf("\n\t%s", __func__);

    uint64_vec_t vec = uint64_vec_init_immed(0);
    assert(uint64_vec_has_uint64(&vec, 0) == false);
    assert(uint64_vec_has_uint64(&vec, 1) == false);
    uint64_vec_free(&vec);
    
    vec = uint64_vec_init_immed(1, 1);
    assert(uint64_vec_has_uint64(&vec, 0) == false);
    assert(uint64_vec_has_uint64(&vec, 1) == true);
    assert(uint64_vec_has_uint64(&vec, 2) == false);
    uint64_vec_free(&vec);
    
    vec = uint64_vec_init_immed(2, 0, 2);
    assert(uint64_vec_has_uint64(&vec, 0) == true);
    assert(uint64_vec_has_uint64(&vec, 1) == false);
    assert(uint64_vec_has_uint64(&vec, 2) == true);
    assert(uint64_vec_has_uint64(&vec, 3) == false);
    uint64_vec_free(&vec);

    assert(clu_mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

    test_utils_cton();
    test_byte_vec_init_immed();
    
    test_utils_uint64_add();
    test_utils_uint64_get_byte();
    test_utils_uint64_set_byte();
    test_utils_uint64_get_size();
    test_utils_uint64_init_byte();
    test_utils_uint128_to_uint64();

    test_utils_byte_vec_init_uint64();
    test_utils_uint64_vec_has_uint64();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
