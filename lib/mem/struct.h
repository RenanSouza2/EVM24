#ifndef __MEM_STRUCT_H__
#define __MEM_STRUCT_H__

#include "header.h"
#include "../bytes/struct.h"

typedef bytes_t mem_t;

mem_t mem_init();
void mem_free(mem_t m);

#endif
