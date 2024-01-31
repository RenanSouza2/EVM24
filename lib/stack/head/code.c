#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#endif

bool stack_push(stack_p s, word_p w)
{
    s->sl = stack_l_create(s->sl, w);
    return true;
}