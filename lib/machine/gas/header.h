#ifndef __GAS_H__
#define __GAS_H__

#include <stdint.h>

#define G_jumpdest  1
#define G_base      2
#define G_very_low  3
#define G_mid       8
#define G_memory    3
#define G_copy      3

uint64_t gas_mem(uint64_t size);
uint64_t gas_copy(uint64_t size);

#endif
