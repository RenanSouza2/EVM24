#include "debug.h"
#include "../../../../mods/clu/header.h"
#include "../../../../mods/macros/assert.h"



#ifdef DEBUG

#include "../../../word/debug.h"



// TODO CLU_HANDLER_IS_SAFE
evm_stack_l_p stack_l_init_variadic(uint64_t n, va_list *args)
{
    evm_stack_l_p sl = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        word_t w = va_arg(*args, word_t);
        sl = stack_l_create(sl, &w);
    }
    return sl;
}

evm_stack_l_p stack_l_init_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return stack_l_init_variadic(n, &args);
}



void stack_l_display(evm_stack_l_p sl)
{
    if(sl == NULL)
        return;

    for(uint64_t i=0; sl; i++, sl = sl->sl)
    {
        printf("\n\tword " U64P() ": ", i);
        word_display(sl->w);
    }
    printf("\n");
}



bool stack_l_test_inner(evm_stack_l_p sl_1, evm_stack_l_p sl_2)
{
    uint64_t i;
    for(i=0; sl_1 && sl_2; i++)
    {
        if(!word_test(sl_1->w, sl_2->w))
        {
            printf("\n\tSTACK LIST ASSERTION ERROR\t| WORD | " U64P(), i);
            return false;
        }

        sl_1 = sl_1->sl;
        sl_2 = sl_2->sl;
    }

    if(sl_2 != NULL)
    {
        printf("\n\n\tSTACK LIST ASSERTION ERROR\t| LIST SHORTER | " U64P() "", i);
        return false;
    }

    if(sl_1 != NULL)
    {
        printf("\n\n\tSTACK LIST ASSERTION ERROR\t| LIST LONGER | " U64P() "", i);
        return false;
    }

    return true;
}

bool stack_l_test(evm_stack_l_p sl_1, evm_stack_l_p sl_2)
{
    if(!stack_l_test_inner(sl_1, sl_2))
    {
        stack_l_display(sl_1);
        stack_l_display(sl_2);
        return false;
    }

    stack_l_free(sl_1);
    stack_l_free(sl_2);
    return true;
}

bool stack_l_immed(evm_stack_l_p sl, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    evm_stack_l_p sl_2 = stack_l_init_variadic(n, &args);
    return stack_l_test(sl, sl_2);
}

#endif



evm_stack_l_p stack_l_create(evm_stack_l_p sl_next, word_p w)
{
    evm_stack_l_p sl = malloc(sizeof(evm_stack_l_t));
    assert(sl);

    *sl = (evm_stack_l_t)
    {
        .sl = sl_next,
        .w = *w
    };
    return sl;
}

evm_stack_l_p stack_l_pop(word_p w, evm_stack_l_p sl)
{
    if(w)
        *w = sl->w;

    assert(sl);
    evm_stack_l_p sl_next = sl->sl;
    free(sl);
    return sl_next;
}

void stack_l_free(evm_stack_l_p sl)
{
    while(sl) sl = stack_l_pop(NULL, sl);
}