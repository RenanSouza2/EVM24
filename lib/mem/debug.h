#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG

#include <stdbool.h>

mem_t mem_init_immed(char str[]);
bool mem_immed(mem_t m, char str[]);

#endif

void mem_expand(mem_p m, int i);

#endif
