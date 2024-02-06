#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG
#endif

void mem_expand(mem_p m, int i);
word_t mem_get_word(mem_p m, int i);

#endif
