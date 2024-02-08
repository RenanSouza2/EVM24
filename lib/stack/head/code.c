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

void stack_display(stack_d_t s)
{
    printf("\nstack");
    printf("\n\tcount: %d", s.count);
    printf("\n");
    stack_l_display(s.sl);
    printf("\n");
}

stack_d_t stack_init_immed(int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_init_immed_variadic(n, &args);
}

stack_d_t stack_init_immed_variadic(int n, va_list *args)
{
    stack_l_p sl = stack_l_init_immed_variadic(n, args);
    return (stack_d_t){sl, n};
}

bool stack_immed(stack_d_t s, int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_immed_variadic(s, n, args);
}

bool stack_immed_variadic(stack_d_t s, int n, va_list args)
{
    if(s.count != n)
    {
        printf("\n\tSTACK ASSERTION ERROR 1 | number of word differ %d %d\t\t", s.count, n);
        return false;
    }

    if(!stack_l_immed_variadic(s.sl, n, args))
    {
        printf("\n\tSTACK ASSERTION ERROR 2 | STACK LIST ASSERTION ERROR\t\t");
        return false;
    }

    return true;
}

bool stack_d_push_immed(stack_d_p s, word_t w)
{
    return stack_d_push(s, &w);
}

#endif



stack_d_t stack_init()
{
    return (stack_d_t){NULL, 0};
}

void stack_free(stack_d_p s)
{
    stack_l_free(s->sl);
}

bool stack_d_push(stack_d_p s, word_p w)
{
    if(s->count == 1024) return false;

    s->count++;
    s->sl = stack_l_create(s->sl, w);
    return true;
}

bool stack_d_pop(word_p w, stack_d_p s)
{
    if(s->count == 0) return false;

    if(w) *w = s->sl->w;
    s->count--;
    s->sl = stack_l_pop(s->sl);
    return true;
}