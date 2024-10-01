#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../utils/debug.h"

evm_rlp_t rlp_init_immed_variadic(uint64_t type, va_list *args)
{
    switch (type)
    {
    case BYTES:;
        char *str = va_arg(*args, char *);
        byte_vec_t b = byte_vec_init_immed(str);
        return rlp_init_byte_vec(&b);

    case LIST:;
        uint64_t size = va_arg(*args, uint64_t);
        evm_rlp_vec_t r = rlp_vec_init(size);
        for (uint64_t i = 0; i < size; i++)
        {
            uint64_t r1_type = va_arg(*args, uint64_t);
            r.arr[i] = rlp_init_immed_variadic(r1_type, args);
        }

        return rlp_init_list(&r);
    }
    assert(false);
}

evm_rlp_t rlp_init_immed(uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    return rlp_init_immed_variadic(type, &args);
}

byte_vec_t rlp_encode_immed(uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    evm_rlp_t r = rlp_init_immed_variadic(type, &args);
    return rlp_encode(&r);
}

uint64_t rlp_decode_immed(evm_rlp_p out_r, char str[])
{
    byte_vec_t b = byte_vec_init_immed(str);
    uint64_t res = rlp_decode(out_r, &b);
    vec_free(&b);
    return res;
}

bool rlp_test_immed(evm_rlp_t r, uint64_t type, ...)
{
    va_list args;
    va_start(args, type);
    evm_rlp_t r_exp = rlp_init_immed_variadic(type, &args);
    return rlp_test(r, r_exp);
    ;
}

bool rlp_test(evm_rlp_t r, evm_rlp_t r_exp)
{
    if (!byte_test(r.type, r_exp.type))
    {
        printf("\n\tRLP ASSERTION ERROR | TYPE");
        return false;
    }

    switch (r.type)
    {
    case BYTES:
        if (!byte_vec_test(r.vec.b, r_exp.vec.b))
        {
            printf("\n\tRLP ASSERTION ERROR | BYTE VEC");
            return false;
        }
        return true;

    case LIST:
        if (!rlp_vec_test(r.vec.r, r_exp.vec.r))
        {
            printf("\n\tRLP ASSERTION ERROR | LIST VEC");
            return false;
        }
        return true;
    }

    printf("\n\n\tRLP ASSERTION ERROR | INVALID TYPE");
    return false;
}

bool rlp_vec_test(evm_rlp_vec_t r, evm_rlp_vec_t r_exp)
{
    if (!uint64_test(r.size, r_exp.size))
    {
        printf("\n\tRLP VECTOR ASSERTION ERROR | SIZE");
        return false;
    }

    for (uint64_t i = 0; i < r.size; i++)
        if (!rlp_test(r.arr[i], r_exp.arr[i]))
        {
            printf("\n\tRLP VECTOR ASSERTION ERROR | ITEM | " U64P, i);
            return false;
        }

    rlp_vec_free_rec(&r_exp);
    return true;
}

#endif

evm_rlp_t rlp_init_byte_vec(byte_vec_p b)
{
    return (evm_rlp_t){BYTES, {.b = *b}};
}

evm_rlp_t rlp_init_list(evm_rlp_vec_p r)
{
    return (evm_rlp_t){LIST, {.r = *r}};
}

evm_rlp_vec_t rlp_vec_init(uint64_t size)
{
    if (size == 0)
        return (evm_rlp_vec_t){0, NULL};

    evm_rlp_p v = calloc(size, sizeof(evm_rlp_t));
    assert(v);

    return (evm_rlp_vec_t){size, v};
}

void rlp_vec_free_rec(evm_rlp_vec_p r)
{
    if(r->size == 0)
        return;

    for (uint64_t i = 0; i < r->size; i++)
        rlp_free(&r->arr[i]);

    free(r->arr);
}

void rlp_free(evm_rlp_p r)
{
    if(r == NULL)
        return;

    switch (r->type)
    {
        case BYTES:
        vec_free(&r->vec.b);
        return;

        case LIST:
        rlp_vec_free_rec(&r->vec.r);
        return;
    }
    assert(false);
}

byte_vec_t rlp_encode(evm_rlp_p r)
{
    switch (r->type)
    {
    case BYTES:;
        byte_vec_t b = r->vec.b;
        if (b.size == 1)
            if (b.arr[0] < 128)
                return b;

        if (b.size < 56)
        {
            byte_vec_t b_size = byte_vec_init_uint64(128 + b.size);
            return byte_vec_concat(&b_size, &b);
        }

        byte_vec_t b_size_2 = byte_vec_init_uint64(b.size);
        byte_vec_t b_size_1 = byte_vec_init_uint64(183 + b_size_2.size);
        b_size_1 = byte_vec_concat(&b_size_1, &b_size_2);
        return byte_vec_concat(&b_size_1, &b);

    case LIST:;
        evm_rlp_vec_t _r = r->vec.r;
        b = byte_vec_init_zero();
        for (uint64_t i = 0; i < _r.size; i++)
        {
            byte_vec_t _b = rlp_encode(&_r.arr[i]);
            b = byte_vec_concat(&b, &_b);
        }
        vec_free(&_r);

        if (b.size < 56)
        {
            byte_vec_t b_size = byte_vec_init_uint64(192 + b.size);
            return byte_vec_concat(&b_size, &b);
        }

        b_size_2 = byte_vec_init_uint64(b.size);
        b_size_1 = byte_vec_init_uint64(247 + b_size_2.size);
        b_size_1 = byte_vec_concat(&b_size_1, &b_size_2);
        return byte_vec_concat(&b_size_1, &b);
    }
    assert(false);
}

uint64_t rlp_get_size_long(
    uint64_p out_head_size,
    uint64_p out_body_size,
    uint64_t size_size,
    byte_p b,
    uint64_t size)
{
    uint64_t head_size = 1 + size_size;
    if (head_size > size)
        return 1;

    uint64_t body_size = uint64_init_byte_arr(&b[1], size_size);
    if (body_size < 56)
        return 2;
        
    *out_head_size = head_size;
    *out_body_size = body_size;
    return 0;
}

uint64_t rlp_get_size(
    uint64_p out_type,
    uint64_p out_head_size, 
    uint64_p out_body_size, 
    byte_p b, 
    uint64_t size
) {
    if (size == 0)
        return 1;

    byte_t b0 = b[0];
    uint64_t type, head_size, body_size;
    switch (b0)
    {
        case 0 ... 127:
        type = BYTES;
        head_size = 0;
        body_size = 1;
        break;
    
        case 128 ... 183:
        type = BYTES;
        head_size = 1;
        body_size = b0 - 128;
        break;

        case 184 ... 192:
        type = BYTES;
        ERR(rlp_get_size_long(&head_size, &body_size, b0 - 183, b, size), 2);
        break;

        case 193 ... 247:
        type = LIST;
        head_size = 1;
        body_size = b0 - 192;
        break;

        case 248 ... 255:
        type = LIST;
        ERR(rlp_get_size_long(&head_size, &body_size, b0 - 247, b, size), 3);
        break;
    }

    if(head_size + body_size > size)
        return 4;
    
    *out_type = type;
    *out_head_size = head_size;
    *out_body_size = body_size;
    return 0;
}

uint64_t rlp_get_r1_sizes(
    uint64_vec_p out_r1_sizes, 
    byte_p b, 
    uint64_t size, 
    uint64_t cnt
) {
    if (size == 0)
    {
        *out_r1_sizes = uint64_vec_init(3 * cnt);
        return 0;
    }

    uint64_t type, head_size, body_size;
    ERR(rlp_get_size(&type, &head_size, &body_size, b, size), 1);
    uint64_t r1_size = head_size + body_size;
    if(r1_size > size)
        return 2;

    ERR(rlp_get_r1_sizes(out_r1_sizes, &b[r1_size], size - r1_size, cnt + 1), 0);

    uint64_t index = 3 * cnt;
    out_r1_sizes->arr[index] = type;
    out_r1_sizes->arr[index + 1] = head_size;
    out_r1_sizes->arr[index + 2] = body_size;
    return 0;
}

evm_rlp_t rlp_decode_bytes(byte_p b, uint64_t body_size)
{
    byte_vec_t _b = byte_vec_init_byte_arr(b, body_size);
    return rlp_init_byte_vec(&_b);
}

uint64_t rlp_decode_list(evm_rlp_p out_r, byte_p b, uint64_t size)
{
    uint64_vec_t r1_sizes;
    ERR(rlp_get_r1_sizes(&r1_sizes, b, size, 0), 1)

    uint64_t r_vec_size = r1_sizes.size / 3;
    evm_rlp_vec_t r_vec = rlp_vec_init(r_vec_size);
    for (uint64_t i = 0, ptr = 0; i < r_vec_size; i++)
    {
        uint64_t index, type, head_size, body_size;
        index = 3 * i;
        type = r1_sizes.arr[index];
        head_size = r1_sizes.arr[index + 1];
        body_size = r1_sizes.arr[index + 2];

        evm_rlp_t r1;
        TRY(rlp_decode_rec(&r1, type, &b[ptr + head_size], body_size))
        {
            rlp_vec_free_rec(&r_vec);
            vec_free(&r1_sizes);
            return ERR_CONCAT(res, 2);
        }
        CATCH

        r_vec.arr[i] = r1;
        ptr += head_size + body_size;
    }
    vec_free(&r1_sizes);

    *out_r = rlp_init_list(&r_vec);
    return 0;
}

uint64_t rlp_decode_rec(evm_rlp_p out_r, uint64_t type, byte_p b, uint64_t size)
{
    switch (type)
    {
        case BYTES:
        *out_r = rlp_decode_bytes(b, size);
        break;
    
        case LIST:
        ERR(rlp_decode_list(out_r, b, size), 1);
        break;

        default:
            return 2;
    }

    return 0;
}

uint64_t rlp_decode(evm_rlp_p out_r, byte_vec_p b)
{
    uint64_t type, head_size, body_size;
    ERR(rlp_get_size(&type, &head_size, &body_size, b->arr, b->size), 1);

    uint64_t rlp_size = head_size + body_size;
    if (rlp_size != b->size)
        return 2;

    evm_rlp_t _r;
    ERR(rlp_decode_rec(&_r, type, &b->arr[head_size], body_size), 3);

    *out_r = _r;
    return 0;
}
