#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define U64_MAX (~(uint64_t)0)
#define U64_FF 0xff00000000000000
#define IGN (-1)
#define IGN_PTR ((handler_p)IGN)

#define NOT_IGNORE(PTR) (((handler_p)PTR) > IGN_PTR)

void uint64_t_display(uint64_t u);

#endif

#endif
