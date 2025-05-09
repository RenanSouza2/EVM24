#include <string.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"



#ifdef DEBUG

#include <stdarg.h>



void byte_vec_display(byte_vec_t b)
{
    CLU_HANDLER_IS_SAFE(b.arr);

    printf("\n\nbytes (" U64P() "):", b.size);
    printf("\n");
    for(uint64_t i = 0; i < b.size >> 5; i++)
    {
        printf("\n\t0x");
        for(uint64_t j = 0; j < 32; j++)
            printf("%02x", b.arr[(i << 5) + j]);
    }
    if(b.size % 32)
    {
        uint64_t i = b.size & ~31;
        printf("\n\t0x");
        for(uint64_t j = i; j < b.size; j++)
            printf("%02x", b.arr[j]);
    }
    printf("\n");
}

void uint64_vec_display(uint64_vec_t b)
{
    CLU_HANDLER_IS_SAFE(b.arr);

    printf("\n\nuint64 (" U64P() "):", b.size);
    printf("\n");
    for(uint64_t i = 0; i < b.size; i++)
    {
        printf("\n\t0x" U64PX "", b.arr[i]);
    }

    printf("\n");
}

byte_t cton(byte_t c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    assert(false);
}

byte_vec_t byte_vec_init_immed(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');
    if(len == 2)
        return byte_vec_init(0);

    assert(len%2 == 0);

    uint64_t size = len / 2 - 1;
    if(size == 0)
        return byte_vec_init_zero();

    byte_vec_t b = byte_vec_init(size);
    for(uint64_t i = 0; i < size; i++)
        b.arr[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return b;
}

uint64_vec_t uint64_vec_init_variadic(uint64_t n, va_list *args)
{
    uint64_vec_t vec = uint64_vec_init(n);
    for(uint64_t i = 0; i < n; i++)
        vec.arr[i] = va_arg(*args, uint64_t);

    return vec;
}

uint64_vec_t uint64_vec_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return uint64_vec_init_variadic(n, &args);
}



bool byte_test(byte_t u1, byte_t u2)
{
    if(u1 == u2)
        return true;

    printf("\n\n\tUCHAR ASSERTION ERROR\t| %d %d", u1, u2);
    return false;
}

bool uint64_test(uint64_t i1, uint64_t i2)
{
    if(i1 != i2)
    {
        printf("\n");
        printf("\n\t0x" U64PX, i1);
        printf("\n\t0x" U64PX, i2);
        printf("\n");
        printf("\n\tUINT64 ASSERTION ERROR");
        return false;
    }

    return true;
}

bool byte_vec_test_inner(byte_vec_t b_1, byte_vec_t b_2)
{
    CLU_HANDLER_IS_SAFE(b_1.arr);
    CLU_HANDLER_IS_SAFE(b_2.arr);

    if(!uint64_test(b_1.size, b_2.size))
    {
        printf("\n\tBYTE VEC ASSERTION ERROR\t| LENGTH");
        return false;
    }

    if(b_1.size == 0)
    {
        if(b_1.arr != NULL)
        {
            printf("\n\n\tBYTE VEC ASSERTION ERROR\t| LENGTH IS ZERO BUT ARR IS NOT NULL");
            return false;
        }
    }

    for(uint64_t i = 0; i < b_1.size; i++)
    {
        if(!byte_test(b_1.arr[i], b_2.arr[i]))
        {
            printf("\n\tBYTE VEC ASSERTION ERROR\t| BYTE | " U64P() "", i);
            return false;
        }
    }

    return true;
}

bool byte_vec_test(byte_vec_t b_1, byte_vec_t b_2)
{
    CLU_HANDLER_IS_SAFE(b_1.arr);
    CLU_HANDLER_IS_SAFE(b_2.arr);

    if(!byte_vec_test_inner(b_1, b_2))
    {
        printf("\n");
        byte_vec_display(b_1);
        byte_vec_display(b_2);
        return false;
    }

    vec_free(&b_1);
    vec_free(&b_2);
    return true;
}

bool uint64_vec_test_inner(uint64_vec_t b_1, uint64_vec_t b_2)
{
    CLU_HANDLER_IS_SAFE(b_1.arr);
    CLU_HANDLER_IS_SAFE(b_2.arr);

    if(!uint64_test(b_1.size, b_2.size))
    {
        printf("\n\tUINT64 VEC TEST ASSERTION ERROR\t| COUNT");
        return false;
    }

    for(uint64_t i = 0; i < b_1.size; i++)
    {
        if(!uint64_test(b_1.arr[i], b_2.arr[i]))
        {
            printf("\n\tUINT64 VEC TEST ASSERTION ERROR\t| UINT64 | " U64P() "", i);
            return false;
        }
    }

    return true;
}

bool uint64_vec_test(uint64_vec_t b_1, uint64_vec_t b_2)
{
    CLU_HANDLER_IS_SAFE(b_1.arr);
    CLU_HANDLER_IS_SAFE(b_2.arr);

    if(!uint64_vec_test_inner(b_1, b_2))
    {
        uint64_vec_display(b_1);
        uint64_vec_display(b_2);
        return false;
    }

    vec_free(&b_1);
    vec_free(&b_2);
    return true;
}

bool byte_vec_immed(byte_vec_t b, char str[])
{
    CLU_HANDLER_IS_SAFE(b.arr);

    byte_vec_t b_exp = byte_vec_init_immed(str);
    return byte_vec_test(b, b_exp);
}

bool uint64_vec_immed(uint64_vec_t b, uint64_t n, ...)
{
    CLU_HANDLER_IS_SAFE(b.arr);

    va_list args;
    va_start(args, n);
    uint64_vec_t b_2 = uint64_vec_init_variadic(n, &args);
    return uint64_vec_test(b, b_2);
}

#endif



uint64_t uint64_add(uint64_t u1, uint64_t u2)
{
    uint64_t sum = u1 + u2;
    return sum < u1 ? UINT64_MAX : sum;
}

byte_t uint64_get_byte(uint64_t u, uint64_t index)
{
    assert(index < 8);
    return (byte_t)(u >> (index << 3));
}

uint64_t uint64_set_byte(uint64_t u, uint64_t index, byte_t b)
{
    assert(index < 8);
    uint64_t offset = index << 3;
    return (u & ~((uint64_t)0xff << offset)) | ((uint64_t)b << offset);
}

uint64_t uint64_get_size(uint64_t u)
{
    for(uint64_t i = 0; i < 8; i++, u >>= 8)
        if(u == 0)
            return i;

    return 8;
}

uint64_t uint64_init_byte_arr(uint64_t size, byte_p arr)
{
    assert(size <= 8);
    uint64_t u = 0;
    for(uint64_t i = 0; i < size; i++)
        u = uint64_set_byte(u, i, arr[size - 1 - i]);

    return u;
}

uint64_t uint128_to_uint64(uint128_t res)
{
    return (res >> 64) ? UINT64_MAX : (uint64_t)res;
}



byte_vec_t byte_vec_init_zero()
{
    return (byte_vec_t)
    {
        .size = 0,
        .arr = NULL
    };
}

byte_vec_t byte_vec_init(uint64_t size)
{
    if(size == 0)
        return byte_vec_init_zero();

    // byte_p arr = calloc(size, sizeof(byte_t));
    byte_p arr = malloc(size * sizeof(byte_t));
    assert(arr);

    return (byte_vec_t)
    {
        .size = size,
        .arr = arr
    };
}

byte_vec_t byte_vec_init_uint64(uint64_t num)
{
    uint64_t size = uint64_get_size(num);
    byte_vec_t b = byte_vec_init(size);

    for(uint64_t i = 0; i < size; i++)
        b.arr[size - 1 - i] = uint64_get_byte(num, i);

    return b;
}

// keeps arr
byte_vec_t byte_vec_init_byte_arr(uint64_t size, byte_p arr)
{
    byte_vec_t b = byte_vec_init(size);
    memcpy(b.arr, arr, size);
    return b;
}

byte_vec_t byte_vec_concat(byte_vec_p b_1, byte_vec_p b_2)
{
    CLU_HANDLER_IS_SAFE(b_1->arr);
    CLU_HANDLER_IS_SAFE(b_2->arr);

    if(b_2->size == 0)
        return *b_1;

    if(b_1->size == 0)
        return *b_2;

    b_1->arr = realloc(b_1->arr, b_1->size + b_2->size);
    memcpy(&b_1->arr[b_1->size], b_2->arr, b_2->size);
    b_1->size += b_2->size;
    vec_free(b_2);
    return *b_1;
}



uint64_vec_t uint64_vec_init_zero()
{
    return (uint64_vec_t)
    {
        .size = 0,
        .arr = NULL
    };
}

uint64_vec_t uint64_vec_init(uint64_t size)
{
    if(size == 0)
        return uint64_vec_init_zero();

    uint64_p arr = calloc(size, sizeof(uint64_t));
    assert(arr);

    return (uint64_vec_t)
    {
        .size = size,
        .arr = arr
    };
}

// vec is assumed to be ordered
bool uint64_vec_has_uint64(uint64_vec_p b, uint64_t v)
{
    CLU_HANDLER_IS_SAFE(b->arr);

    if(b->size == 0)
        return false;

    uint64_t min = 0;
    for(uint64_t max = b->size; max - min > 1;)
    {
        uint64_t mid = (min + max) >> 1;
        uint64_t _v = b->arr[mid];
        if(_v == v)
            return true;

        if(_v > v)
            max = mid;
        else
            min = mid;
    }
    return b->arr[min] == v;
}



void vec_free(handler_p v)
{
    CLU_HANDLER_IS_SAFE(VEC(v)->arr);

    if(VEC(v)->arr)
        free(VEC(v)->arr);
}
 