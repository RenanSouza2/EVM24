#ifndef __FRAME_DEBUG_H__
#define __FRAME_DEBUG_H__

#include "struct.h"
#include "../gas/header.h"

#ifdef DEBUG

#define GAS_DEF 100000000

evm_frame_t frame_init_immed(char str_code[], int gas);
evm_frame_t frame_init_immed_setup(char str_code[], int gas, int n_mem, ...);

bool frame_test_immed(evm_frame_t f, int pc, int gas, int n_mem, ...);

#endif

evm_frame_t frame_init(evm_bytes_t code, int gas);
void frame_free(evm_frame_t f);

evm_frame_o_t frame_stop(evm_frame_p f);
bool frame_pop(evm_frame_p f);
bool frame_mstore(evm_frame_p f);
bool frame_push(evm_frame_p f);

#endif
