#ifndef __FRAME_DEBUG_H__
#define __FRAME_DEBUG_H__

#include "header.h"
#include "struct.h"

#ifdef DEBUG

#define GAS_DEF 100000000

evm_frame_t frame_init_immed(char str_code[],
    uint64_t pc,
    uint64_t gas,
    uint64_t n_mem,
    ...
);
uint64_vec_t frame_get_jumpdest_immed(char str_code[]);
void frame_populate_stack(evm_frame_p f, uint64_t max);

bool frame_test(evm_frame_t f_1, evm_frame_t f_2);
bool frame_immed(evm_frame_t f,
    char str_code[],
    uint64_t pc,
    uint64_t gas,
    uint64_t n_mem,
    ...
);
bool frame_o_immed(evm_frame_o_t fo, bool success, uint64_t gas, char str_returndata[]);

#endif

#define STOP 0x00
#define ADD  0x01

#define CODESIZE 0x38

#define POP     0x50
#define MLOAD   0x51
#define MSTORE  0x52
#define MSTORE8 0x53

#define JUMP    0x56

#define JUMPDEST 0x5b

#define PUSH0   0x5f
#define PUSH32  0x7f

#define RETURN  0xf3

evm_frame_t frame_init(evm_bytes_t code, uint64_t gas);
uint64_vec_t frame_get_jumpdest(evm_bytes_p code);

void frame_free(evm_frame_p f);
void frame_o_free(evm_frame_o_p fo);

evm_frame_o_t frame_stop(evm_frame_p f);

int frame_pop(evm_frame_p f);
int frame_mload(evm_frame_p f);
int frame_mstore(evm_frame_p f);
int frame_mstore8(evm_frame_p f);

int frame_push(evm_frame_p f);

evm_frame_o_t frame_return(evm_frame_p f);

#endif
