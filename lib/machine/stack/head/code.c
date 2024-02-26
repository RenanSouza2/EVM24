#include <stdio.h>

#include "debug.h"
#include "../list/header.h"



#ifdef DEBUG

#include "../list/debug.h"
#include "../../../utils/debug.h"

void stack_display(evm_stack_t s)
{
    printf("\nstack");
    printf("\n\tcount: " U64P, s.count);
    printf("\n");
    stack_l_display(s.sl);
    printf("\n");
}

evm_stack_t stack_init_immed_variadic(int n, va_list *args)
{
    evm_stack_l_p sl = stack_l_init_immed_variadic(n, args);
    return (evm_stack_t){sl, n};
}



bool stack_test_immed(evm_stack_t s, int n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_test_variadic(s, n, &args);
}

bool stack_test_variadic(evm_stack_t s, int n, va_list *args)
{
    if(!uint64_test(s.count, n))
    {
        printf("\n\tSTACK ASSERTION ERROR | COUNT");
        return false;
    }

    if(!stack_l_test_variadic(s.sl, n, args))
    {
        printf("\n\tSTACK ASSERTION ERROR | STACK LIST");
        return false;
    }

    return true;
}



int stack_push_immed(evm_stack_p s, evm_word_t w)
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

int stack_push(evm_stack_p s, evm_word_p w)
{
    if(s->count == 1024) return 1;

    s->count++;
    s->sl = stack_l_create(s->sl, w);
    return 0;
}

int stack_pop(evm_word_p w, evm_stack_p s)
{
    if(s->count == 0) return 1;

    s->count--;
    s->sl = stack_l_pop(w, s->sl);
    return 0;
}