#ifndef __FRAME_DEBUG_H__
#define __FRAME_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

frame_t frame_init_immed(char str_code[]);
frame_t frame_init_immed_setup(char str_code[], char str_mem[], int n, ...);

bool frame_immed(frame_t f, int pc, char str_mem[], int n, ...);

#endif

frame_t frame_init(bytes_t code);
void frame_free(frame_t f);

frame_o_t frame_stop(frame_p f);
bool frame_pop(frame_p f);
bool frame_mstore(frame_p f);
bool frame_push(frame_p f);

#endif
