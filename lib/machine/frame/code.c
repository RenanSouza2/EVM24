#include <stdio.h>

#include "debug.h"
#include "../../../utils/assert.h"

#include "../gas/header.h"



#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#include "../mem/debug.h"
#include "../stack/head/debug.h"
#include "../bytes/debug.h"
#include "../../utils/debug.h"



evm_frame_t frame_init_immed(char str_code[], uint64_t gas)
{
    evm_bytes_t code = byte_vec_init_immed(str_code);
    return frame_init(code, gas);
}

evm_frame_t frame_init_immed_setup(char str_code[], uint64_t gas, uint64_t n_mem, ...)
{
    evm_bytes_t code = byte_vec_init_immed(str_code);

    va_list args;
    va_start(args, n_mem);
    return (evm_frame_t)
    {
        0,
        gas,
        code,
        frame_get_jumpdest(&code),
        mem_init_immed_variadic(n_mem, &args),
        stack_init_immed_variadic(va_arg(args, uint64_t), &args)
    };
}

uint64_vec_t frame_get_jumpdest_immed(char str_code[])
{
    evm_bytes_t code = byte_vec_init_immed(str_code);
    uint64_vec_t jumpdest = frame_get_jumpdest(&code);
    vec_free(VEC(&code));
    return jumpdest;
}



bool frame_test_immed(evm_frame_t f, uint64_t pc, uint64_t gas, uint64_t n_mem, ...) {
    va_list args;
    va_start(args, n_mem);

    if(pc < IGN)
    if(!uint64_test(f.pc, pc))
    {
        printf("\n\tFRAME ASSERTION ERROR | PC");
        return false;
    }

    if(gas < IGN)
    if(!uint64_test(f.gas, gas))
    {
        printf("\n\tFRAME ASSERTION ERROR | GAS");
        return false;
    }

    if(n_mem < IGN)
    if(!mem_test_variadic(f.m, n_mem, &args))
    {
        printf("\n\tFRAME ASSERTION ERROR | MEM");
        return false;
    }

    uint64_t n_stack = va_arg(args, uint64_t);
    if(n_stack < IGN)
    if(!stack_test_variadic(f.s, n_stack, &args))
    {
        printf("\n\tFRAME ASSERTION ERROR | STACK");
        return false;
    }

    return true;
}

bool frame_o_test_immed(evm_frame_o_t fo, bool success, uint64_t gas, char str_returndata[])
{
    if(fo.success != success) {
        printf("\n\n\tFRAME ASSERTION ERROR | SUCCESS");
        return false;
    }

    if(!uint64_test(fo.gas, gas))
    {
        printf("\n\tFRAME OUTPUT ASSERTION ERROR | GAS");
        return false;
    }

    if(!byte_vec_test_immed(fo.returndata, str_returndata))
    {
        printf("\n\tFRAME OUTPUT ASSERTION ERROR | RETURN DATA");
        return false;
    }

    return true;
}

#endif



#define GAS_VERIFY(GAS, CODE) if(f->gas < (GAS)) return CODE
#define GAS_CONSUME(GAS) f->gas -= (GAS);



evm_frame_t frame_init(evm_bytes_t code, uint64_t gas)
{
    return (evm_frame_t)
    {
        0,
        gas,
        code,
        frame_get_jumpdest(&code),
        byte_vec_init_zero(),
        stack_init(),
    };
}

uint64_vec_t frame_get_jumpdest(evm_bytes_p code) // TODO improve test
{
    uint64_t count = 0;
    for(uint64_t pc=0; pc<code->size; pc++)
    {
        byte_t op = bytes_get_byte(code, pc);
        switch (op)
        {
            case JUMPDEST: count++; break;
            case PUSH0 ... PUSH32: pc += op - PUSH0;
        }
    }
    if(count == 0) return uint64_vec_init_clean(0);

    uint64_vec_t vec = uint64_vec_init_clean(count);
    count = 1;
    for(uint64_t pc=0; pc<code->size; pc++)
    {
        byte_t op = bytes_get_byte(code, pc);
        switch (op)
        {
            case JUMPDEST: vec.arr[count++] = pc; break;
            case PUSH0 ... PUSH32: pc += op - PUSH0; break;
        }
    }
    return vec;
}



void frame_free(evm_frame_p f)
{
    vec_free(VEC(&f->code));
    vec_free(VEC(&f->jumpdest));
    stack_free(&f->s);
    vec_free(VEC(&f->m));
}

void frame_o_free(evm_frame_o_p fo)
{
    vec_free(VEC(&fo->returndata));
}



byte_t frame_get_op(evm_frame_p f)
{
    return bytes_get_byte(&f->code, f->pc);
}

int frame_push_uint64(evm_frame_p f, uint64_t value)
{
    GAS_VERIFY(G_base, 1);
    GAS_CONSUME(G_base);

    word_t w = word_init_uint64(value);
    if(stack_push(&f->s, &w)) return 2;
    f->pc++;

    return 0;
}



evm_frame_o_t frame_stop(evm_frame_p f)
{
    return (evm_frame_o_t){true, f->gas, byte_vec_init_zero()};
}

evm_frame_o_t frame_halt(evm_frame_p f)
{
    return (evm_frame_o_t){false, f->gas, byte_vec_init_zero()};
}



int frame_add(evm_frame_p f) // TODO test
{
    word_t w1, w2;
    if(stack_pop(&w1, &f->s)) return 1;
    if(stack_pop(&w2, &f->s)) return 2;

    word_t w = word_add(&w1, &w2);
    assert(!stack_push(&f->s, &w));
    f->pc++;

    GAS_VERIFY(G_very_low, 3);
    GAS_CONSUME(G_very_low);
    return 0;
}



int frame_codesize(evm_frame_p f) // TODO test
{
    GAS_VERIFY(G_base, 1);
    GAS_CONSUME(G_base);

    word_t w = word_init_uint64(f->code.size);
    if(stack_push(&f->s, &w)) return 2;
    return 0;
}

int frame_codecopy(evm_frame_p f) // TODO test
{
    word_t w_mem, w_code, w_size;
    if(stack_pop(&w_mem , &f->s)) return 1;
    if(stack_pop(&w_code, &f->s)) return 2;
    if(stack_pop(&w_size, &f->s)) return 3;
    if(!word_is_uint64(&w_size))  return 4;

    uint64_t size = w_size.arr[0];
    uint64_t gas_mem = mem_dry_run(&f->m, w_mem, size);
    uint64_t gas_cpy = gas_copy(size);
    uint64_t gas = uint64_add(gas_mem, gas_cpy);
    GAS_VERIFY(gas, 4);
    GAS_CONSUME(gas);

    evm_bytes_t b = bytes_get_bytes(&f->code, w_code.arr[0], size);
    mem_set_bytes(&f->m, w_mem.arr[0], &b);
    vec_free(VEC(&b));
    return 0;
}



int frame_pc(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->pc);
}

int frame_msize(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->m.size);
}

int frame_gas(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->gas - G_base);
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
    word_t w_pos;
    if(stack_pop(&w_pos, &f->s)) return 1;
    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 32);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_VERIFY(gas, 2);
    GAS_CONSUME(gas);

    word_t w_value = mem_get_word(&f->m, w_pos.arr[0]);
    assert(!stack_push(&f->s, &w_value));
    f->pc++;

    return 0;
}

int frame_mstore(evm_frame_p f)
{
    word_t w_pos, w_value;
    if(stack_pop(&w_pos, &f->s)) return 1;
    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 32);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_VERIFY(gas, 2);
    
    if(stack_pop(&w_value, &f->s)) return 3;
    GAS_CONSUME(gas);

    mem_set_word(&f->m, w_pos.arr[0], &w_value);
    f->pc++;
    
    return 0;
}

int frame_mstore8(evm_frame_p f)
{
    word_t w_pos, w_value;
    if(stack_pop(&w_pos, &f->s))   return 1;
    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 1);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_VERIFY(gas, 2);

    if(stack_pop(&w_value, &f->s)) return 3;
    GAS_CONSUME(gas);
    
    byte_t u = word_get_byte(&w_value, 0);
    mem_set_byte(&f->m, w_pos.arr[0], u);
    f->pc++;
    
    return 0;
}



int frame_jump(evm_frame_p f) // TODO test
{
    word_t w_pos;
    if(stack_pop(&w_pos, &f->s)) return 1;
    if(!word_is_uint64(&w_pos))  return 2;
    uint64_t pos = w_pos.arr[0];

    if(!uint64_vec_has_uint64(&f->jumpdest, pos)) return 3;

    GAS_VERIFY(G_mid, 4);
    GAS_CONSUME(G_mid);

    f->pc = pos;
    return 0;
}



int frame_jumpdest(evm_frame_p f) // TODO test
{
    GAS_VERIFY(G_jumpdest, 1);
    GAS_CONSUME(G_jumpdest);

    f->pc++;
    return 0;
}



int frame_push(evm_frame_p f)
{
    byte_t op = frame_get_op(f);
    assert(0x59 <= op);
    assert(op <= 0x7f);
    int size = op - 0x5f;
    int gas = size ? G_very_low : G_base;
    GAS_VERIFY(gas, 1);
 
    evm_bytes_t b = bytes_get_bytes(&f->code, f->pc+1, size);
    word_t w = word_init_bytes(&b);
    if(stack_push(&f->s, &w)) return 2;
    f->pc += 1 + size;

    GAS_CONSUME(gas);
    return 0;
}



evm_frame_o_t frame_return(evm_frame_p f)
{
    word_t w_ptr;
    if(stack_pop(&w_ptr, &f->s)) return frame_halt(f);
    
    word_t w_size;
    if(stack_pop(&w_size, &f->s)) return frame_halt(f);
    if(!word_is_uint64(&w_size)) return frame_halt(f);
    uint64_t gas = mem_dry_run(&f->m, w_ptr, w_size.arr[0]);
    GAS_VERIFY(gas, frame_halt(f));
    GAS_CONSUME(gas);

    return (evm_frame_o_t){true, f->gas, mem_get_bytes(&f->m, w_ptr.arr[0], w_size.arr[0])};
}



#define REV(FN) if(FN(&f)) return frame_halt(&f); break

evm_frame_o_t frame_execute(evm_bytes_t code, uint64_t gas)
{
    evm_frame_t f = frame_init(code, gas);

    while(true)
    switch (frame_get_op(&f))
    {
        case STOP: return frame_stop(&f);
        case ADD : REV(frame_add);

        case POP    : REV(frame_pop);
        case MLOAD  : REV(frame_mload);
        case MSTORE : REV(frame_mstore);

        case PUSH0 ... PUSH32: REV(frame_push);

        case RETURN: return frame_return(&f);

        default: assert(false);
    }
}
