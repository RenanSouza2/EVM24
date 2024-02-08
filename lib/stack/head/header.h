#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include <stdbool.h>

#include "../../../utils/struct.h"
#include "../../word/header.h"

PLACEHOLDER(stack_evm);

void stack_free(stack_evm_p s);
bool stack_evm_push(stack_evm_p s, word_p w);
bool stack_evm_pop(word_p w, stack_evm_p s);

#endif
