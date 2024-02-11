#ifndef __GAS_H__
#define __GAS_H__

#include <stdint.h>

#include "../../utils/struct.h"

#define G_base      2
#define G_very_low  3
#define G_memory    3

uint64_t gas_mem(uint64_t size);

#endif
