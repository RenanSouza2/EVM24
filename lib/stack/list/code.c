#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"
#include "../../word/debug.h"

void stack_l_display(stack_l_p sl)
{
    for(int i=0; sl; i++, sl = sl->sl)
    {
        printf("\n\tword %d: ", i);
        word_display(sl->w);
    }
    printf("\n");
}

bool stack_l(stack_l_p sl, int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_l(sl, n, args);
}

bool stack_l_var(stack_l_p sl, int n, va_list args)
{
    int i;
    for(i=0; sl && i < n; sl = sl->sl, i++)
    {
        word_t w = va_arg(args, word_t);
        if(!word_eq_bool(&sl->w, &w))
        {
            printf("\nSTACK LIST ASSERTION ERROR 1 | word %d differed\t\t", i);
            return false;
        }
    }

    if(i < n)
    {
        printf("\nSTACK LIST ASSERTION ERROR 2 | less words than expected | %d %d\t\t", i, n);
        return false;
    }

    if(sl != NULL)
    {
        printf("\nSTACK LIST ASSERTION ERROR 3 | more words than expected | %d\t\t", n);
        return false;
    }

    return true;
}

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

void stack_l_free(stack_l_p sl)
{
    while(sl) sl = stack_l_pop(sl);
}