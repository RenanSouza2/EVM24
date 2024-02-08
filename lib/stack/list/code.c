#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include <stdarg.h>

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

stack_l_p stack_l_init_immed_variadic(int n, va_list *args)
{
    stack_l_p sl = NULL;
    for(int i=0; i<n; i++)
    {
        word_t w = va_arg(*args, word_t);
        sl = stack_l_create(sl, &w);
    }
    return sl;
}

bool stack_l_immed(stack_l_p sl, int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_l_immed_variadic(sl, n, args);
}

bool stack_l_immed_variadic(stack_l_p sl, int n, va_list args)
{
    int i;
    for(i=0; sl && i < n; sl = sl->sl, i++)
    {
        word_t w = va_arg(args, word_t);
        if(!word(sl->w, w))
        {
            printf("\n\tSTACK LIST ASSERTION ERROR 1 | WORD ASSERTION ERROR | %d\t\t", i);
            return false;
        }
    }

    if(i < n)
    {
        printf("\n\n\tSTACK LIST ASSERTION ERROR 2 | LESS WORDS ERROR | %d %d\t\t", i, n);
        return false;
    }

    if(sl != NULL)
    {
        printf("\n\n\tSTACK LIST ASSERTION ERROR 3 | MORE WORDS ERROR | %d\t\t", n);
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