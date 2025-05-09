#include "debug.h"
#include "../../../../mods/clu/header.h"

#include "../list/header.h"



#ifdef DEBUG

#include "../list/debug.h"
#include "../../../utils/debug.h"

void stack_display(evm_stack_t s)
{
    printf("\nstack");
    printf("\n\tcount: " U64P() "", s.count);
    printf("\n");
    stack_l_display(s.sl);
    printf("\n");
}

evm_stack_t stack_init_variadic(uint64_t n, va_list *args)
{
    evm_stack_l_p sl = stack_l_init_variadic(n, args);
    return (evm_stack_t)
    {
        .sl = sl,
        .count = n
    };
}

evm_stack_t stack_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_init_variadic(n, &args);
}



bool stack_test_inner(evm_stack_t s_1, evm_stack_t s_2)
{
    if(!uint64_test(s_2.count, s_2.count))
    {
        printf("\n\tSTACK ASSERTION ERROR\t| COUNT");
        return false;
    }

    if(!stack_l_test(s_1.sl, s_2.sl))
    {
        printf("\n\tSTACK ASSERTION ERROR\t| STACK LIST");
        return false;
    }

    return true;
}

bool stack_test(evm_stack_t s_1, evm_stack_t s_2)
{
    if(!stack_test_inner(s_1, s_2))
    {
        stack_display(s_1);
        stack_display(s_2);
        return false;
    }

    return true;
}

bool stack_immed(evm_stack_t s, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    evm_stack_t s_2 = stack_init_variadic(n, &args);
    return stack_test(s, s_2);
}




uint64_t stack_push_immed(evm_stack_p s, word_t w)
{
    return stack_push(s, &w);
}

#endif



evm_stack_t stack_init()
{
    return (evm_stack_t)
    {
        .sl = NULL,
        .count = 0
    };
}

void stack_free(evm_stack_p s)
{
    stack_l_free(s->sl);
}

// returns 1 if faillure, 0 if success
uint64_t stack_push(evm_stack_p s, word_p w)
{
    if(s->count == 1024)
        return 1;

    s->count++;
    s->sl = stack_l_create(s->sl, w);
    return 0;
}

uint64_t stack_pop(word_p w, evm_stack_p s)
{
    if(s->count == 0)
        return 1;

    s->count--;
    s->sl = stack_l_pop(w, s->sl);
    return 0;
}