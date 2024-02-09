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

void stack_display(evm_stack_t s)
{
    printf("\nstack");
    printf("\n\tcount: %d", s.count);
    printf("\n");
    stack_l_display(s.sl);
    printf("\n");
}

evm_stack_t stack_init_immed(int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_init_immed_variadic(n, &args);
}

evm_stack_t stack_init_immed_variadic(int n, va_list *args)
{
    stack_l_p sl = stack_l_init_immed_variadic(n, args);
    return (evm_stack_t){sl, n};
}

bool stack_immed(evm_stack_t s, int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_immed_variadic(s, n, args);
}

bool stack_immed_variadic(evm_stack_t s, int n, va_list args)
{
    if(s.count != n)
    {
        printf("\n\n\tSTACK ASSERTION ERROR | number of word differ %d %d\t\t", s.count, n);
        return false;
    }

    if(!stack_l_immed_variadic(s.sl, n, args))
    {
        printf("\n\tSTACK ASSERTION ERROR | STACK LIST ASSERTION ERROR\t\t");
        return false;
    }

    return true;
}

bool stack_evm_push_immed(evm_stack_p s, evm_word_t w)
{
    return stack_push(s, &w);
}

#endif



evm_stack_t stack_init()
{
    return (evm_stack_t){NULL, 0};
}

void stack_free(evm_stack_p s)
{
    stack_l_free(s->sl);
}

bool stack_push(evm_stack_p s, evm_word_p w)
{
    if(s->count == 1024) return false;

    s->count++;
    s->sl = stack_l_create(s->sl, w);
    return true;
}

bool stack_pop(evm_word_p w, evm_stack_p s)
{
    if(s->count == 0) return false;

    if(w) *w = s->sl->w;
    s->count--;
    s->sl = stack_l_pop(s->sl);
    return true;
}