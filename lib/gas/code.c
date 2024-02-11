#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../utils/struct.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



uint64_t gas_mem(uint64_t size)
{
    uint128_t size128 = (uint128_t)size;
    uint128_t res = G_memory * size128 +  ((size128 * size128) >> 9);
    return  (res >> 64) ? UINT64_MAX : (uint64_t) res;
}