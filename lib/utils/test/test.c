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

    for(int i=0; i<16; i++)
        assert(cton(ascii[i]) == i);

    char ASCII[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for(int i=0; i<6; i++)
        assert(cton(ASCII[i]) == i + 10);

    assert(clu_mem_empty());
}

void test_byte_vec_init_immed()
{
    printf("\n\t%s", __func__);

    printf("\n\t\t%s  1", __func__);
    byte_vec_t b = byte_vec_init_immed("0x");
    assert(b.v == NULL);
    assert(b.size == 0);

    printf("\n\t\t%s  2", __func__);
    b = byte_vec_init_immed("0x00");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 0);
    byte_vec_free(&b);

    printf("\n\t\t%s  3", __func__);
    b = byte_vec_init_immed("0x01");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 1);
    byte_vec_free(&b);

    printf("\n\t\t%s  4", __func__);
    b = byte_vec_init_immed("0x0f");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 15);
    byte_vec_free(&b);

    printf("\n\t\t%s  5", __func__);
    b = byte_vec_init_immed("0x10");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 16);
    byte_vec_free(&b);

    printf("\n\t\t%s  6", __func__);
    b = byte_vec_init_immed("0x80");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 128);
    byte_vec_free(&b);

    printf("\n\t\t%s  7", __func__);
    b = byte_vec_init_immed("0xf0");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 240);
    byte_vec_free(&b);

    printf("\n\t\t%s  8", __func__);
    b = byte_vec_init_immed("0xff");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 255);
    byte_vec_free(&b);

    printf("\n\t\t%s  9", __func__);
    b = byte_vec_init_immed("0x0000");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 0);
    assert(b.v[1] == 0);
    byte_vec_free(&b);

    printf("\n\t\t%s 10", __func__);
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
    test_utils_uint128_to_uint64();
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
