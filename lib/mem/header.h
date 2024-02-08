#ifndef __MEM_H__
#define __MEM_H__

#include "../../utils/struct.h"
#include "../bytes/header.h"

typedef bytes_p mem_p;

word_t mem_get_word(mem_p m, int i);
void mem_set_word(mem_p m, int i, word_p w);

#endif
