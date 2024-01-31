#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include <stdbool.h>

#include "../../../utils/struct.h"
#include "../../word/header.h"

PLACEHOLDER(stack);

void stack_free(stack_p s);
bool stack_push(stack_p s, word_p w);
bool stack_pop(word_p w, stack_p s);

#endif
