#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

int gas_mem(int size)
{
    return G_memory * size + ((size * size) >> 9); 
}