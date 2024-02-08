#ifndef __STACK_HEAD_H__
#define __STACK_HEAD_H__

#include <stdbool.h>

#include "../../../utils/struct.h"
#include "../../word/header.h"

PLACEHOLDER(stack_d);

void stack_free(stack_d_p s);
bool stack_d_push(stack_d_p s, word_p w);
bool stack_d_pop(word_p w, stack_d_p s);

#endif
