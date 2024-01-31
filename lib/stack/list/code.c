#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#endif

stack_l_p stack_l_create(stack_l_p sl_next, word_p w)
{
    stack_l_p sl = malloc(sizeof(stack_l_t));
    assert(sl);

    *sl = (stack_l_t){sl_next, *w};
    return sl;
}

stack_l_p stack_l_pop(stack_l_p sl)
{
    assert(sl);
    stack_l_p sl_next = sl->sl;
    free(sl);
    return sl_next;
}