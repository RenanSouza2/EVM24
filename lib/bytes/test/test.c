#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"

void test_bytes_cton()
{
    printf("\n\t%s\t\t", __func__);

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

    assert(mem_empty());
}

void test_bytes_init_immed()
{
    printf("\n\t%s\t\t", __func__);

    bytes_t b = bytes_init_immed("0x");
    assert(b.v == NULL);
    assert(b.size == 0);

    b = bytes_init_immed("0x00");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 0);
    bytes_free(b);

    b = bytes_init_immed("0x01");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 1);
    bytes_free(b);

    b = bytes_init_immed("0x0f");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 15);
    bytes_free(b);

    b = bytes_init_immed("0x10");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 16);
    bytes_free(b);

    b = bytes_init_immed("0x80");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 128);
    bytes_free(b);

    b = bytes_init_immed("0xf0");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 240);
    bytes_free(b);

    b = bytes_init_immed("0xff");
    assert(b.size == 1);
    assert(b.v);
    assert(b.v[0] == 255);
    bytes_free(b);

    b = bytes_init_immed("0x0000");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 0);
    assert(b.v[1] == 0);
    bytes_free(b);

    b = bytes_init_immed("0x1234");
    assert(b.size == 2);
    assert(b.v);
    assert(b.v[0] == 18);
    assert(b.v[1] == 52);
    bytes_free(b);

    assert(mem_empty());
}

void test_bytes()
{
    printf("\n%s\t\t", __func__);

    test_bytes_cton();
    test_bytes_init_immed();

    assert(mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
