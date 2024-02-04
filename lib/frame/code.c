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



// void frame_execute(bytes_t code)
// {
//     frame_t f = frame_init(code);
// }
