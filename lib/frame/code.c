#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"
#include "../bytes/debug.h"

frame_t frame_init_immed(char str_code[])
{
    bytes_t code = bytes_init_immed(str_code);
    return frame_init(code);
}

#endif



frame_t frame_init(bytes_t code)
{
    return (frame_t){
        0,
        code,
        stack_init(),
        mem_init()
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

bool frame_push(frame_p f)
{
    uchar op = frame_get_op(f);
    int size = op - 0x5f;
    bytes_t b = bytes_get_bytes(&f->code, f->pc+1, size);
    word_t w = word_from_bytes(&b);
    if(!stack_push(&f->s, &w)) return false;
    f->pc += 1 + size;
    return true;
}

frame_o_t frame_execute(bytes_t code)
{
    frame_t f = frame_init(code);

    while(true)
    {
        switch (frame_get_op(&f))
        {
            case 0: return frame_stop(&f);

            default: assert(false);
        }
    }
}
