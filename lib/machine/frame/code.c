#include "debug.h"
#include "../../../mods/clu/header.h"
#include "../../../mods/macros/assert.h"

#include "../gas/header.h"
#include "../bytes/header.h"
#include "../mem/header.h"
#include "../stack/head/header.h"
#include "../../utils/header.h"
#include "../../word/header.h"



#ifdef DEBUG

#include "../mem/debug.h"
#include "../stack/head/debug.h"
#include "../../utils/debug.h"



evm_frame_t frame_init_variadic(
    char str_code[],
    uint64_t pc,
    uint64_t gas,
    uint64_t n_mem,
    va_list *args
)
{
    evm_bytes_t code = byte_vec_init_immed(str_code);

    return (evm_frame_t)
    {
        pc,
        gas,
        code,
        frame_get_jumpdest(&code),
        mem_init_variadic(n_mem, args),
        stack_init_variadic(va_arg(*args, uint64_t), args)
    };
}

evm_frame_t frame_init_immed(
    char str_code[],
    uint64_t pc,
    uint64_t gas,
    uint64_t n_mem,
    ...
)
{
    va_list args;
    va_start(args, n_mem);
    return frame_init_variadic(str_code, pc, gas, n_mem, &args);
}

uint64_vec_t frame_get_jumpdest_immed(char str_code[])
{
    evm_bytes_t code = byte_vec_init_immed(str_code);
    uint64_vec_t jumpdest = frame_get_jumpdest(&code);
    vec_free(&code);
    return jumpdest;
}

void frame_stack_populate(evm_frame_p f, uint64_t max)
{
    for(uint64_t i=0; i<max; i++)
        assert(!stack_push_immed(&f->s, W1(i)));
}



bool frame_test(evm_frame_t f_1, evm_frame_t f_2)
{
    if(!uint64_test(f_1.pc, f_2.pc))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| PC");
        return false;
    }

    if(!uint64_test(f_1.gas, f_2.gas))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| GAS");
        return false;
    }

    if(!byte_vec_test(f_1.code, f_2.code))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| CODE");
        return false;
    }

    if(!uint64_vec_test(f_1.jumpdest, f_2.jumpdest))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| JUMPDEST");
        return false;
    }

    if(!mem_test(f_1.m, f_2.m))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| MEM");
        return false;
    }

    if(!stack_test(f_1.s, f_2.s))
    {
        printf("\n\tFRAME ASSERTION ERROR\t| STACK");
        return false;
    }

    return true;
}

bool frame_immed(evm_frame_t f,
    char str_code[],
    uint64_t pc,
    uint64_t gas,
    uint64_t n_mem,
    ...
)
{
    va_list args;
    va_start(args, n_mem);
    evm_frame_t f_2 = frame_init_variadic(str_code, pc, gas, n_mem, &args);
    return frame_test(f, f_2);
}

bool frame_o_immed(evm_frame_o_t fo, bool success, uint64_t gas, char str_returndata[])
{
    if(fo.success != success)
    {
        printf("\n\n\tFRAME OUTPUT ASSERTION ERROR\t| SUCCESS FLAG");
        return false;
    }

    if(!uint64_test(fo.gas, gas))
    {
        printf("\n\tFRAME OUTPUT ASSERTION ERROR\t| GAS");
        return false;
    }

    if(!byte_vec_immed(fo.returndata, str_returndata))
    {
        printf("\n\tFRAME OUTPUT ASSERTION ERROR\t| RETURN DATA");
        return false;
    }

    return true;
}

#endif



#define GAS_CONSUME(GAS, CODE)  \
    {                           \
        if(f->gas < U64(GAS))   \
            return CODE;        \
        f->gas -= (GAS);        \
    }



uint64_vec_t frame_get_jumpdest(evm_bytes_p code)
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
    if(count == 0)
        return uint64_vec_init(0);

    uint64_vec_t vec = uint64_vec_init(count);
    count = 0;
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

void frame_free(evm_frame_p f)
{
    vec_free(&f->code);
    vec_free(&f->jumpdest);
    vec_free(&f->m);
    stack_free(&f->s);
}

void frame_o_free(evm_frame_o_p fo)
{
    vec_free(&fo->returndata);
}



byte_t frame_get_opcode(evm_frame_p f)
{
    uint64_t pc = f->pc;
    f->pc++;
    return bytes_get_byte(&f->code, pc);
}

uint64_t frame_push_uint64(evm_frame_p f, uint64_t value)
{
    GAS_CONSUME(G_base, 1);

    word_t w = W1(value);
    ERR(2, stack_push(&f->s, &w));

    return 0;
}



evm_frame_o_t frame_STOP(evm_frame_p f)
{
    return (evm_frame_o_t)
    {
        .success =  true,
        .gas = f->gas,
        .returndata = byte_vec_init_zero()
    };
}

evm_frame_o_t frame_INVALID(evm_frame_p f)
{
    frame_free(f);
    return (evm_frame_o_t)
    {
        .success = false,
        .gas = 0,
        .returndata = byte_vec_init_zero()
    };
}



uint64_t frame_ADD(evm_frame_p f) // TODO test
{
    word_t w1, w2;
    ERR(1, stack_pop(&w1, &f->s));
    ERR(2, stack_pop(&w2, &f->s));

    word_t w = word_add(&w1, &w2);
    assert(!stack_push(&f->s, &w));

    GAS_CONSUME(G_very_low, 3);
    return 0;
}



uint64_t frame_CODESIZE(evm_frame_p f) // TODO test
{
    GAS_CONSUME(G_base, 1);

    word_t w = W1(f->code.size);
    ERR(2, stack_push(&f->s, &w));
    return 0;
}

uint64_t frame_CODECOPY(evm_frame_p f) // TODO test
{
    word_t w_mem, w_code, w_size;
    ERR(1, stack_pop(&w_mem , &f->s));
    ERR(2, stack_pop(&w_code, &f->s));
    ERR(3, stack_pop(&w_size, &f->s));
    ERR(4, !word_is_uint64(&w_size));

    uint64_t size = w_size.arr[0];
    uint64_t gas_mem = mem_dry_run(&f->m, w_mem, size);
    uint64_t gas_cpy = gas_copy(size);
    uint64_t gas = uint64_add(gas_mem, gas_cpy);
    GAS_CONSUME(gas, 4);

    evm_bytes_t b = bytes_get_bytes(&f->code, w_code.arr[0], size);
    mem_set_bytes(&f->m, w_mem.arr[0], &b);
    vec_free(&b);
    return 0;
}



uint64_t frame_PC(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->pc);
}

uint64_t frame_MSIZE(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->m.size);
}

uint64_t frame_GAS(evm_frame_p f) // TODO test
{
    return frame_push_uint64(f, f->gas - G_base);
}



uint64_t frame_POP(evm_frame_p f)
{
    GAS_CONSUME(G_base, 1);

    ERR(2, stack_pop(NULL, &f->s));

    return 0;
}

uint64_t frame_MLOAD(evm_frame_p f)
{
    word_t w_pos;
    ERR(1, stack_pop(&w_pos, &f->s));
    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 32);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_CONSUME(gas, 2);

    word_t w_value = mem_get_word(&f->m, w_pos.arr[0]);
    assert(!stack_push(&f->s, &w_value));

    return 0;
}

uint64_t frame_MSTORE(evm_frame_p f)
{
    word_t w_pos, w_value;
    ERR(1, stack_pop(&w_pos, &f->s));
    ERR(2, stack_pop(&w_value, &f->s))

    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 32);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_CONSUME(gas, 3);

    mem_set_word(&f->m, w_pos.arr[0], &w_value);

    return 0;
}

uint64_t frame_MSTORE8(evm_frame_p f)
{
    word_t w_pos, w_value;
    ERR(1, stack_pop(&w_pos, &f->s));
    ERR(2, stack_pop(&w_value, &f->s));

    uint64_t gas_expand = mem_dry_run(&f->m, w_pos, 1);
    uint64_t gas = uint64_add(G_very_low, gas_expand);
    GAS_CONSUME(gas, 3);

    byte_t u = word_get_byte(&w_value, 31);
    mem_set_byte(&f->m, w_pos.arr[0], u);

    return 0;
}



uint64_t frame_JUMP(evm_frame_p f) // TODO test
{
    word_t w_pos;
    ERR(1, stack_pop(&w_pos, &f->s));

    ERR(2, !word_is_uint64(&w_pos));
    uint64_t pos = w_pos.arr[0];
    ERR(3, !uint64_vec_has_uint64(&f->jumpdest, pos));

    GAS_CONSUME(G_mid, 4);
    f->pc = pos;
    return 0;
}



uint64_t frame_JUMPDEST(evm_frame_p f) // TODO test
{
    GAS_CONSUME(G_jumpdest, 1);

    return 0;
}



uint64_t frame_PUSH(evm_frame_p f, byte_t opcode)
{
    assert(0x59 <= opcode);
    assert(opcode <= 0x7f);
    uint64_t size = opcode - 0x5f;
    uint64_t gas = size ? G_very_low : G_base;
    GAS_CONSUME(gas, 1);

    evm_bytes_t b = bytes_get_bytes(&f->code, f->pc, size);
    word_t w = word_init_bytes(&b);
    ERR(2, stack_push(&f->s, &w));
    f->pc += size;

    return 0;
}



evm_frame_o_t frame_RETURN(evm_frame_p f)
{
    word_t w_pos;
    if(stack_pop(&w_pos, &f->s))
        return frame_INVALID(f);

    word_t w_size;
    if(stack_pop(&w_size, &f->s))
        return frame_INVALID(f);

    if(!word_is_uint64(&w_size))
        return frame_INVALID(f);

    uint64_t size = w_size.arr[0];
    uint64_t gas = mem_dry_run(&f->m, w_pos, size);
    GAS_CONSUME(gas, frame_INVALID(f));

    evm_frame_o_t res = (evm_frame_o_t)
    {
        .success = true,
        .gas = f->gas,
        .returndata = mem_get_bytes(&f->m, w_pos.arr[0], size)
    };
    frame_free(f);
    return res;
}



#define CASE_EXIT(OPCODE) case OPCODE: return frame_##OPCODE(&f);
#define CASE_OP(OPCODE) case OPCODE: if(frame_##OPCODE(&f)) return frame_INVALID(&f); break;
#define CASE_SPAN(OPCODE, BEGIN, END)   \
    case OPCODE##BEGIN ... OPCODE##END: if(frame_##OPCODE(&f, opcode)) return frame_INVALID(&f); break;

evm_frame_o_t frame_execute(evm_bytes_t code, uint64_t gas)
{
    evm_frame_t f = frame_init(code, gas);

    while(true)
    {
        byte_t opcode = frame_get_opcode(&f);
        switch (opcode)
        {
            CASE_EXIT(STOP)
            CASE_OP(ADD)

            CASE_OP(CODESIZE)

            CASE_OP(POP)
            CASE_OP(MLOAD)
            CASE_OP(MSTORE)
            CASE_OP(MSTORE8)

            CASE_OP(JUMP)

            CASE_OP(PC)
            CASE_OP(MSIZE)
            CASE_OP(GAS)
            CASE_OP(JUMPDEST)

            CASE_SPAN(PUSH, 0, 32)

            CASE_EXIT(RETURN)

            CASE_EXIT(INVALID)
        }

        exit(EXIT_FAILURE);
    }
}
