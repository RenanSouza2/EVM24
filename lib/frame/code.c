#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "debug.h"
#include "../bytes/struct.h"
#include "../mem/header.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"
#include "../stack/head/debug.h"
#include "../bytes/debug.h"
#include "../mem/debug.h"



frame_t frame_init_immed(char str_code[], int gas)
{
    bytes_t code = bytes_init_immed(str_code);
    return frame_init(code, gas);
}

frame_t frame_init_immed_setup(char str_code[], int gas, char str_mem[], int n, ...)
{
    va_list args;
    va_start(args, n);
    return (frame_t)
    {
        0,
        gas,
        bytes_init_immed(str_code),
        stack_init_immed_variadic(n, &args),
        mem_init_immed(str_mem)
    };
}

bool frame_immed(frame_t f, int pc, char str_mem[], int n, ...)
{
    if(f.pc != pc)
    {
        printf("\n\n\tFRAME ASSERTION ERROR 1 | PC DID NOT MATCH | %d %d\n\n", f.pc, pc);
        return false;
    }

    if(str_mem)
    if(!mem_immed(f.m, str_mem))
    {
        printf("\n\tFRAME ASSERTION ERROR 2 | MEM ASSERTION ERROR\n\n");
        return false;
    }

    va_list args;
    va_start(args, n);
    if(!stack_immed_variadic(f.s, n, args))
    {
        printf("\n\tFRAME ASSERTION ERROR 3 | MEM ASSERTION ERROR\n\n");
        return false;
    }

    return true;
}

#endif



frame_t frame_init(bytes_t code, int gas)
{
    return (frame_t)
    {
        0,
        gas,
        code,
        stack_init(),
        mem_init(),
    };
}

void frame_free(frame_t f)
{
    bytes_free(&f.code);
    stack_free(&f.s);
    mem_free(f.m);
}

uchar frame_get_op(frame_p f)
{
    return bytes_get_byte(&f->code, f->pc);
}



frame_o_t frame_stop(frame_p f)
{
    return (frame_o_t){bytes_init_zero()};
}

frame_o_t frame_halt(frame_p f)
{
    return (frame_o_t){bytes_init_zero()};
}

bool frame_pop(frame_p f)
{
    if(!stack_evm_pop(NULL, &f->s)) return false;

    f->pc++;
    return true;
}

bool frame_mload(frame_p f) // TODO test
{
    word_t w_pos;
    if(!stack_evm_pop(&w_pos, &f->s)) return false;

    word_t  w_value;
    w_value = mem_get_word(&f->m, w_pos.v[0]);

    if(!stack_evm_push(&f->s, &w_value)) return false;
    f->pc++;

    return true;
}

bool frame_mstore(frame_p f)
{
    word_t w_pos, w_value;
    if(!stack_evm_pop(&w_pos, &f->s)) return false;
    if(!stack_evm_pop(&w_value, &f->s)) return false;

    // TODO (what?)
    mem_set_word(&f->m, w_pos.v[0], &w_value);
    f->pc++;
    
    return true;
}

bool frame_push(frame_p f)
{
    uchar op = frame_get_op(f);
    assert(0x59 <= op);
    assert(op <= 0x7f);

    int size = op - 0x5f;
    bytes_t b = bytes_get_bytes(&f->code, f->pc+1, size);
    word_t w = word_from_bytes(&b);
    if(!stack_evm_push(&f->s, &w)) return false;
    f->pc += 1 + size;
    return true;
}



#define REV(FN) if(!FN(&f)) return frame_halt(&f)

frame_o_t frame_execute(bytes_t code, int gas)
{
    frame_t f = frame_init(code, gas);

    while(true)
    {
        switch (frame_get_op(&f))
        {
            case 0: return frame_stop(&f);

            case 0x50: REV(frame_pop);
            case 0x51: REV(frame_mload);
            case 0x52: REV(frame_mstore);

            case 0x5f ... 0x7f: REV(frame_push);

            default: assert(false);
        }
    }
}
