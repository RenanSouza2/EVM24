#ifndef __FRAME_STRUCT_H__
#define __FRAME_STRUCT_H__

#include "header.h"
#include "../stack/head/struct.h"
#include "../mem/struct.h"

STRUCT(evm_frame)
{
    int pc;
    int gas;
    evm_bytes_t code;
    evm_stack_t s;
    evm_mem_t m;
};

STRUCT(evm_frame_o)
{
    evm_bytes_t returndata;
    int gas;
};

#endif
