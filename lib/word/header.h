#ifndef __WORD_H__
#define __WORD_H__

#include "../../utils/struct.h"
#include "../utils/struct.h"

PLACEHOLDER(word);

uchar word_get_byte(word_p w, int i);
void word_set_byte(word_p w, int i, uchar u);

#endif
