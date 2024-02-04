#ifndef __FRAME_DEBUG_H__
#define __FRAME_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

frame_t frame_init_immed(char str_code[]);

#endif

frame_t frame_init(bytes_t code);
void frame_free(frame_t f);

#endif
