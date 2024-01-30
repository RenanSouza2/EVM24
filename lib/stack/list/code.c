#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#endif

s_list_p s_list_create(s_list_p sl_next, word_t w)
{
    s_list_p sl = malloc(sizeof(s_list_t));
    assert(sl);

    *sl = (s_list_t){sl_next, w};
    return sl;
}
