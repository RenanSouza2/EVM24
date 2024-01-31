#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"
#include "../list/struct.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../../utils/clu/bin/header.h"
#include "../../word/debug.h"
#include "../list/debug.h"

void stack_display(stack_t s)
{
    printf("\nstack");
    printf("\n\tcout: %d", s.count);
    printf("\n");
    stack_l_display(s.sl);
    printf("\n");
}

bool stack(stack_t s, int n, ...)
{
    if(s.count != n)
    {
        printf("\nSTACK ASSERTION ERROR 1 | number of word differ %d %d\t\t", s.count, n);
        return false;
    }

    va_list args;
    va_start(args, n);
    if(!stack_l_var(s.sl, n, args))
    {
        printf("\nSTACK ASSERTION ERROR 2 | STACK LIST ASSERTION ERROR\t\t");
        return false;
    }

    return true;
}

bool stack_push_immed(stack_p s, word_t w)
{
    return stack_push(s, &w);
}

#endif

stack_t stack_initialize()
{
    return (stack_t){NULL, 0};
}

void stack_free(stack_p s)
{
    stack_l_free(s->sl);
}

bool stack_push(stack_p s, word_p w)
{
    if(s->count == 1024) return false;

    s->count++;
    s->sl = stack_l_create(s->sl, w);
    return true;
}

bool stack_pop(word_p w, stack_p s)
{
    if(s->count == 0) return false;

    *w = s->sl->w;
    s->count--;
    s->sl = stack_l_pop(s->sl);
    return true;
}