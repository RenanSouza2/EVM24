#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include <stdbool.h>

#include "../../../utils/struct.h"
#include "../../word/header.h"

PLACEHOLDER(evm_stack);

void stack_free(evm_stack_p s);
bool stack_push(evm_stack_p s, evm_word_p w);
bool stack_pop(evm_word_p w, evm_stack_p s);

#endif
