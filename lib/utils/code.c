#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

void uint64_t_display(uint64_t u)
{
    printf("0x");
    for(int i=0; i<64; i++)
        printf("%02hu", ((uchar*)&u)[i]);
}
