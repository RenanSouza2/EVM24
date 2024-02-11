#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"
#include "../bytes/struct.h"
#include "../mem/header.h"
#include "../utils/header.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../stack/head/debug.h"
#include "../bytes/debug.h"
#include "../mem/debug.h"
#include "../utils/debug.h"


evm_frame_t frame_init_immed(char str_code[], int gas)
{
    evm_bytes_t code = bytes_init_immed(str_code);
    return frame_init(code, gas);
}

evm_frame_t frame_init_immed_setup(char str_code[], int gas, int n_mem, ...)
{
    va_list args;
    va_start(args, n_mem);
    return (evm_frame_t)
    {
        0,
        gas,
        bytes_init_immed(str_code),
        mem_init_immed_variadic(n_mem, &args),
        stack_init_immed_variadic(va_arg(args, int), &args)
    };
}



bool frame_test_immed(evm_frame_t f, int pc, int gas, int n_mem, ...) {
    va_list args;
    va_start(args, n_mem);

    if(pc > IGN)
    if(!int_test(f.pc, pc))
    {
        printf("\n\tFRAME ASSERTION ERROR | PC\t\t");
        return false;
    }

    if(gas > IGN)
    if(!int_test(f.gas, gas))
    {
        printf("\n\tFRAME ASSERTION ERROR | GAS\t\t");
        return false;
    }

    if(n_mem > IGN)
    if(!mem_test_variadic(f.m, n_mem, &args))
    {
        printf("\n\tFRAME ASSERTION ERROR | MEM\n\n");
        return false;
    }

    int n_stack = va_arg(args, int);
    if(n_stack > IGN)
    if(!stack_test_variadic(f.s, n_stack, &args))
    {
        printf("\n\tFRAME ASSERTION ERROR | STACK\n\n");
        return false;
    }

    return true;
}

#endif



#define GAS_VERIFY_DEPRECATED(GAS) if(f->gas < GAS) return false // TODO delete
#define GAS_VERIFY(GAS, CODE) if(f->gas < GAS) return CODE
#define GAS_CONSUME(GAS) f->gas -= GAS;



evm_frame_t frame_init(evm_bytes_t code, int gas)
{
    return (evm_frame_t)
    {
        0,
        gas,
        code,
        mem_init(),
        stack_init(),
    };
}

void frame_free(evm_frame_t f)
{
    bytes_free(&f.code);
    stack_free(&f.s);
    mem_free(f.m);
}

uchar frame_get_op(evm_frame_p f)
{
    return bytes_get_byte(&f->code, f->pc);
}



evm_frame_o_t frame_stop(evm_frame_p f)
{
    return (evm_frame_o_t){bytes_init_zero()};
}

evm_frame_o_t frame_halt(evm_frame_p f)
{
    return (evm_frame_o_t){bytes_init_zero()};
}



int frame_pop(evm_frame_p f)
{
    GAS_VERIFY(G_base, 1);

    if(stack_pop(NULL, &f->s)) return 2;
    f->pc++;

    GAS_CONSUME(G_base);
    return 0;
}

int frame_mload(evm_frame_p f)
{
    evm_word_t w_pos;
    if(stack_pop(&w_pos, &f->s)) return 1;
    if(!word_is_uint_64(&w_pos)) return 2;

    int gas = mem_dry_run(&f->m, w_pos.v[0] + 32);
    GAS_VERIFY(gas, 3);
    GAS_CONSUME(gas);

    evm_word_t w_value = mem_get_word(&f->m, w_pos.v[0]);
    assert(!stack_push(&f->s, &w_value));
    f->pc++;

    return 0;
}

int frame_mstore(evm_frame_p f) // TODO gas
{
    evm_word_t w_pos, w_value;
    if(stack_pop(&w_pos, &f->s))   return 1;
    if(!word_is_uint_64(&w_pos))    return 2;

    if(stack_pop(&w_value, &f->s)) return 3;

    int gas = mem_dry_run(&f->m, w_pos.v[0]+32);
    GAS_VERIFY(gas, 4);
    GAS_CONSUME(gas);

    mem_set_word(&f->m, w_pos.v[0], &w_value);
    f->pc++;
    
    return 0;
}



int frame_push(evm_frame_p f)
{
    GAS_VERIFY(G_very_low, 1);

    uchar op = frame_get_op(f);
    assert(0x59 <= op);
    assert(op <= 0x7f);

    int size = op - 0x5f;
    evm_bytes_t b = bytes_get_bytes(&f->code, f->pc+1, size);
    evm_word_t w = word_init_bytes(&b);
    if(stack_push(&f->s, &w)) return 2;
    f->pc += 1 + size;

    GAS_CONSUME(G_very_low);
    return 0;
}



#define REV(FN) if(FN(&f)) return frame_halt(&f)

evm_frame_o_t frame_execute(evm_bytes_t code, int gas)
{
    evm_frame_t f = frame_init(code, gas);

    while(true)
    switch (frame_get_op(&f))
    {
        case STOP: return frame_stop(&f);

        case POP    : REV(frame_pop);
        case MLOAD  : REV(frame_mload);
        case MSTORE : REV(frame_mstore);

        case PUSH0 ... PUSH32: REV(frame_push);

        default: assert(false);
    }
}
