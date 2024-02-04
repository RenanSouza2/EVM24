#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG
#endif

word_t mem_get_word(mem_p m, int i);
void mem_set_word(mem_p m, int i, word_p w);

#endif
