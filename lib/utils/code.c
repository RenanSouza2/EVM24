#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"

void byte_vec_display(byte_vec_t b)
{
    printf("\n\nbytes (" U64P "):", b.size);
    printf("\n");
    for (uint64_t i = 0; i < b.size >> 5; i++)
    {
        printf("\n\t0x");
        for (uint64_t j = 0; j < 32; j++)
            printf("%02x", b.arr[(i << 5) + j]);
    }
    if (b.size % 32)
    {
        uint64_t i = b.size & ~31;
        printf("\n\t0x");
        for (uint64_t j = i; j < b.size; j++)
            printf("%02x", b.arr[j]);
    }
    printf("\n");
}

byte_t cton(byte_t c)
{
    switch (c)
    {
    case '0' ... '9':
        return c - '0';
    case 'a' ... 'f':
        return c - 'a' + 10;
    case 'A' ... 'F':
        return c - 'A' + 10;
    }
    assert(false);
}

byte_vec_t byte_vec_init_immed(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1);
    assert(str[0] == '0');
    assert(str[1] == 'x');
    if (len == 2)
        return byte_vec_init(0);

    uint64_t size = len / 2 - 1;
    byte_t *b = malloc(size);
    for (uint64_t i = 0; i < size; i++)
        b[i] = (cton(str[2 * i + 2]) << 4) | cton(str[2 * i + 3]);

    return (byte_vec_t){size, b};
}

uint64_vec_t uint64_vec_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    uint64_vec_t vec = uint64_vec_init_clean(n);
    for (uint64_t i = 0; i < n; i++)
        vec.arr[i] = va_arg(args, uint64_t);

    return vec;
}

uint64_t uint64_init_byte_immed(char str[])
{
    byte_vec_t b = byte_vec_init_immed(str);
    uint64_t res = uint64_init_byte_vec(b.arr, b.size);
    vec_free(&b);
    return res;
}

bool byte_test(byte_t u1, byte_t u2)
{
    if (u1 == u2)
        return true;

    printf("\n\n\tUCHAR ASSERTION ERROR | %d %d", u1, u2);
    return false;
}

bool uint64_test(uint64_t i1, uint64_t i2)
{
    if (i1 == i2)
        return true;

    printf("\n");
    printf("\n\t0x" U64PX, i1);
    printf("\n\t0x" U64PX, i2);
    printf("\n");
    printf("\n\tUINT64 ASSERTION ERROR");
    return false;
}

bool byte_vec_test(byte_vec_t b, byte_vec_t b_exp)
{
    if (!uint64_test(b.size, b_exp.size))
    {
        printf("\n\tBYTE VEC ASSERTION ERROR | LENGTH");
        vec_free(&b_exp);
        return false;
    }

    for (uint64_t i = 0; i < b.size; i++)
    {
        if (!byte_test(b.arr[i], b_exp.arr[i]))
        {
            printf("\n\tBYTE VEC ASSERTION ERROR | BYTE | " U64P, i);
            vec_free(&b_exp);
            return false;
        }
    }

    vec_free(&b_exp);
    return true;
}

bool byte_vec_test_immed(byte_vec_t b, char str[])
{
    byte_vec_t b_exp = byte_vec_init_immed(str);
    return byte_vec_test(b, b_exp);
}

bool uint64_vec_test_immed(uint64_vec_t vec, uint64_t n, ...)
{
    if (!uint64_test(vec.size, n))
    {
        printf("\n\tUINT64 VEC TEST ASSERTION ERROR | COUNT");
        return false;
    }

    va_list args;
    va_start(args, n);
    for (uint64_t i = 0; i < n; i++)
    {
        uint64_t jumpdest = va_arg(args, uint64_t);
        if (!uint64_test(vec.arr[i], jumpdest))
        {
            printf("\n\tUINT64 VEC TEST ASSERTION ERROR | UINT64 | " U64P, i);
            return false;
        }
    }

    return true;
}

#endif

#pragma region uint

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
    for (uint64_t i = 0; i < 8; i++, u >>= 8)
        if (u == 0)
            return i;

    return 8;
}

uint64_t uint64_init_byte_vec(byte_p b, uint64_t size)
{
    assert(size <= 8);
    uint64_t u = 0;
    for (uint64_t i = 0; i < size; i++)
        u = uint64_set_byte(u, i, b[size - 1 - i]);
    return u;
}

uint64_t uint128_to_uint64(uint128_t res)
{
    return (res >> 64) ? UINT64_MAX : (uint64_t)res;
}

#pragma endregion uint

#pragma region vec

byte_vec_t byte_vec_init_zero()
{
    return (byte_vec_t){0, NULL};
}

byte_vec_t byte_vec_init_uint64(uint64_t u)
{
    uint64_t size = uint64_get_size(u);
    byte_vec_t b = byte_vec_init(size);
    for (uint64_t i = 0; i < size; i++)
        b.arr[size - 1 - i] = uint64_get_byte(u, i);
    return b;
}

byte_vec_t byte_vec_init(uint64_t size)
{
    if (size == 0)
        return byte_vec_init_zero();
    byte_p v = calloc(size, sizeof(byte_t));
    assert(v);

    return (byte_vec_t){size, v};
}

uint64_vec_t uint64_vec_init_clean(uint64_t size)
{
    if (size == 0)
        return (uint64_vec_t){0, NULL};
    uint64_p v = calloc(size, sizeof(uint64_t));
    assert(v);

    return (uint64_vec_t){size, v};
}

void vec_free(handler_p v)
{
    if (VEC(v)->arr)
        free(VEC(v)->arr);
}

byte_vec_t byte_vec_concat(byte_vec_p b1, byte_vec_p b2) // TODO test
{
    if (b2->size == 0)
        return *b1;
    if (b1->size == 0)
        return *b2;

    b1->arr = realloc(b1->arr, b1->size + b2->size);
    memcpy(&b1->arr[b1->size], b2->arr, b2->size);
    b1->size += b2->size;
    vec_free(b2);
    return *b1;
}

bool uint64_vec_has_uint64(uint64_vec_p vec, uint64_t v)
{
    if (vec->size == 0)
        return false;

    uint64_t min = 0;
    for (uint64_t max = vec->size; max - min > 1;)
    {
        uint64_t mid = (min + max) >> 1;
        uint64_t _v = vec->arr[mid];
        if (_v == v)
            return true;

        if (_v > v)
            max = mid;
        else
            min = mid;
    }
    return vec->arr[min] == v;
}

#pragma endregion vec
