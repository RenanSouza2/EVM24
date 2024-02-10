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
#include "../utils/debug.h"


evm_frame_t frame_init_immed(char str_code[], int gas)
{
    evm_bytes_t code = bytes_init_immed(str_code);
    return frame_init(code, gas);
}

evm_frame_t frame_init_immed_setup(char str_code[], int gas, char str_mem[], int n, ...)
{
    va_list args;
    va_start(args, n);
    return (evm_frame_t)
    {
        0,
        gas,
        bytes_init_immed(str_code),
        stack_init_immed_variadic(n, &args),
        mem_init_immed(str_mem)
    };
}

bool frame_test_immed(evm_frame_t f, int pc, int gas, char str_mem[], int n, ...)
{
    if(pc > IGN)
    if(f.pc != pc)
    {
        printf("\n\n\tFRAME ASSERTION ERROR | PC DID NOT MATCH | %d %d\n\n", f.pc, pc);
        return false;
    }

    if(gas > IGN)
    if(f.gas != gas)
    {
        printf("\n\n\tFRAME ASSERTION ERROR | GAS DID NOT MATCH | %d %d\n\n", f.gas, gas);
        return false;
    }

    if(NOT_IGNORE(str_mem))
    if(!mem_test_immed(f.m, str_mem))
    {
        printf("\n\tFRAME ASSERTION ERROR | MEM ASSERTION ERROR\n\n");
        return false;
    }

    if(n > IGN)
    {
        va_list args;
        va_start(args, n);
        if(!stack_test_variadic(f.s, n, args))
        {
            printf("\n\tFRAME ASSERTION ERROR | MEM ASSERTION ERROR\n\n");
            return false;
        }
    }

    return true;
}

#endif



#define GAS_VERIFY(GAS) if(f->gas < GAS) return false

#define GAS_CONSUME(GAS)                \
    {                                   \
        if(f->gas < GAS) return false;  \
        f->gas -= GAS;                  \
    }



evm_frame_t frame_init(evm_bytes_t code, int gas)
{
    return (evm_frame_t)
    {
        0,
        gas,
        code,
        stack_init(),
        mem_init(),
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

bool frame_pop(evm_frame_p f)
{
    if(!stack_pop(NULL, &f->s)) return false;
    f->pc++;

    GAS_CONSUME(G_base);
    return true;
}

bool frame_mload(evm_frame_p f) // TODO test
{
    evm_word_t w_pos;
    if(!stack_pop(&w_pos, &f->s)) return false;

    for(int i=1; i<V_MAX; i++)
        if(w_pos.v[i])
            return false;

    int gas = mem_dry_run(&f->m, w_pos.v[0]);
    GAS_CONSUME(gas);

    evm_word_t w_value = mem_get_word(&f->m, w_pos.v[0]);
    assert(stack_push(&f->s, &w_value));
    f->pc++;

    return true;
}

bool frame_mstore(evm_frame_p f)
{
    evm_word_t w_pos, w_value;
    if(!stack_pop(&w_pos, &f->s)) return false;
    if(!stack_pop(&w_value, &f->s)) return false;

    // TODO(?) (requires w_pos < 2 ** 32)
    mem_set_word(&f->m, w_pos.v[0], &w_value);
    f->pc++;
    
    return true;
}

bool frame_push(evm_frame_p f)
{
    GAS_CONSUME(G_very_low);

    uchar op = frame_get_op(f);
    assert(0x59 <= op);
    assert(op <= 0x7f);

    int size = op - 0x5f;
    evm_bytes_t b = bytes_get_bytes(&f->code, f->pc+1, size);
    evm_word_t w = word_from_bytes(&b);
    if(!stack_push(&f->s, &w)) return false;
    f->pc += 1 + size;
    return true;
}



#define REV(FN) if(!FN(&f)) return frame_halt(&f)

evm_frame_o_t frame_execute(evm_bytes_t code, int gas)
{
    evm_frame_t f = frame_init(code, gas);

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
