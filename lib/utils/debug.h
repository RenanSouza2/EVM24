#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define U64_MAX (~(uint64_t)0)
#define U64_FF 0xff00000000000000

void uint64_t_display(uint64_t u);

#endif

#endif
