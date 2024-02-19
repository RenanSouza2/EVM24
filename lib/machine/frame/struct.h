#ifndef __FRAME_STRUCT_H__
#define __FRAME_STRUCT_H__

#include "header.h"

#include "../mem/struct.h"
#include "../stack/head/struct.h"

STRUCT(evm_frame)
{
    uint64_t pc;
    uint64_t gas;
    evm_bytes_t code;
    uint64_vec_t jumpdest;
    evm_mem_t m;
    evm_stack_t s;
};

STRUCT(evm_frame_o) // cbAIH%cV45dOiIP1&uAi
{
    bool success;
    uint64_t gas;
    evm_bytes_t returndata;
};

#endif
