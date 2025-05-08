#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include "../../mods/macros/assert.h"

#define U128(U641, U642) ((((uint128_t)(U641)) << 64) | (uint128_t)(U642))

#define U64_MAX     UINT64_MAX
#define U64_FF      0xff00000000000000
#define U128_1      U128(1, 0)
#define U128_MAX    U128(UINT64_MAX, UINT64_MAX)

#define IGN UINT64_MAX

#define assert_byte(RES, EXP) assert(byte_test(RES, EXP))
#define assert_64(RES, EXP) assert(uint64_test(RES, EXP))

void byte_vec_display(byte_vec_t b);

byte_t cton(byte_t c);
byte_vec_t byte_vec_init_immed(char str[]);
uint64_vec_t uint64_vec_init_immed(uint64_t n, ...);
uint64_t uint64_init_byte_immed(char str[]);

bool byte_test(byte_t u1, byte_t u2);
bool uint64_test(uint64_t i1, uint64_t i2);

bool byte_vec_test_inner(byte_vec_t b_1, byte_vec_t b_2);
bool byte_vec_test(byte_vec_t b, byte_vec_t b_exp);
bool uint64_vec_test(uint64_vec_t vec_1, uint64_vec_t vec_2);
bool byte_vec_immed(byte_vec_t b, char str[]);
bool uint64_vec_immed(uint64_vec_t vec, uint64_t n, ...);

#endif

byte_t uint64_get_byte(uint64_t u, uint64_t index);
uint64_t uint64_set_byte(uint64_t u, uint64_t index, byte_t b);
uint64_t uint64_get_size(uint64_t u);

#endif
