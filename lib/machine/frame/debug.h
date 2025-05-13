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
void frame_stack_populate(evm_frame_p f, uint64_t max);

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

#define STOP            0x00
#define ADD             0x01

#define BYTE            0x1a    //  TODO

#define ADDRESS         0x30    //  TODO ENV

#define CALLDATALOAD    0x35    //  TODO CALLDATA
#define CALLDATASIZE    0x36    //  TODO CALLDATA
#define CALLDATACOPY    0x37    //  TODO CALLDATA
#define CODESIZE        0x38    //  TODO CODE
#define CODECOPY        0x39    //  TODO CODE

#define RETURNDATASIZE  0x3d    //  TODO RETURNDATABUFFER
#define RETURNDATACOPY  0x3e    //  TODO RETURNDATABUFFER

#define POP             0x50
#define MLOAD           0x51
#define MSTORE          0x52
#define MSTORE8         0x53

#define JUMP            0x56
#define JUMPI           0x57    // TODO
#define PC              0X58
#define MSIZE           0x59
#define GAS             0x5a
#define JUMPDEST        0x5b

#define PUSH0           0x5f
#define PUSH32          0x7f

#define DUP1            0x80    //  TODO
#define DUP16           0x8f    //  TODO
#define SWAP1           0x90    //  TODO
#define SWAP16          0x9f    //  TODO

#define RETURN          0xf3
#define REVERT          0xfd    //  TODO
#define INVALID         0xfe

uint64_vec_t frame_get_jumpdest(evm_bytes_p code);
evm_frame_t frame_init(evm_bytes_t code, uint64_t gas);
void frame_free(evm_frame_p f);
void frame_o_free(evm_frame_o_p fo);

byte_t frame_get_opcode(evm_frame_p f);
uint64_t frame_push_uint64(evm_frame_p f, uint64_t value);

evm_frame_o_t frame_STOP(evm_frame_p f);

uint64_t frame_POP(evm_frame_p f);
uint64_t frame_MLOAD(evm_frame_p f);
uint64_t frame_MSTORE(evm_frame_p f);
uint64_t frame_MSTORE8(evm_frame_p f);

uint64_t frame_PUSH(evm_frame_p f, byte_t opcode);

evm_frame_o_t frame_RETURN(evm_frame_p f);

#endif
