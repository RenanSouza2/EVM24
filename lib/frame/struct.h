#ifndef __FRAME_STRUCT_H__
#define __FRAME_STRUCT_H__

#include "header.h"
#include "../stack/head/struct.h"
#include "../mem/struct.h"

STRUCT(frame)
{
    int pc;
    int gas;
    bytes_t code;
    stack_evm_t s;
    mem_t m;
};

STRUCT(frame_o)
{
    bytes_t returndata;
    int gas;
};

#endif
