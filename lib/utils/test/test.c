#include "../debug.h"
#include "../../../utils/clu/bin/header.h"



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
    vec_free(&vec);
    
    vec = uint64_vec_init_immed(1, 1);
    assert(uint64_vec_has_uint64(&vec, 0) == false);
    assert(uint64_vec_has_uint64(&vec, 1) == true);
    assert(uint64_vec_has_uint64(&vec, 2) == false);
    vec_free(&vec);
    
    vec = uint64_vec_init_immed(2, 0, 2);
    assert(uint64_vec_has_uint64(&vec, 0) == true);
    assert(uint64_vec_has_uint64(&vec, 1) == false);
    assert(uint64_vec_has_uint64(&vec, 2) == true);
    assert(uint64_vec_has_uint64(&vec, 3) == false);
    vec_free(&vec);

    assert(clu_mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

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
