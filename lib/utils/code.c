#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

void uint64_t_display(uint64_t u)
{
    for(int i=7; i>=0; i--)
        printf("%02hx", ((uchar*)&u)[i]);
}
