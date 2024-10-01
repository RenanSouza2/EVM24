#ifndef __NODE_STRUCT_H__
#define __NODE_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"
#include "../../word/struct.h"

#define LEAF        0
#define BRANCH      1
#define EXTENSION   2

#define NL(POINTER) ((node_leaf_p)(POINTER))
#define NB(POINTER) ((node_branch_p)(POINTER))
#define NE(POINTER) ((node_extension_p)(POINTER))

STRUCT(node)
{
    uint64_t type;
};

STRUCT(node_leaf)
{
    node_t n;
    byte_vec_t path;
    byte_vec_t v;
};

STRUCT(evm_node_branch)
{
    node_t n;
    word_t next[16];
    byte_vec_p v;
};

STRUCT(evm_node_extension)
{
    node_t n;
    byte_vec_t path;
    word_t next;
};

#endif
