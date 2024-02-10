#ifndef __BYTES_DEBUG_H__
#define __BYTES_DEBUG_H__

#include "struct.h"
#include "../word/struct.h"

#ifdef DEBUG

#include <stdbool.h>

uchar cton(uchar c);
void bytes_display(evm_bytes_t b);
evm_bytes_t bytes_init_immed(char str[]);

bool bytes_test_immed(evm_bytes_t b, char str[]);
bool bytes_test(evm_bytes_t b, evm_bytes_t b_exp);

#endif

#endif
