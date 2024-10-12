#ifndef __NODE_STRUCT_H__
#define __NODE_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"
#include "../../word/struct.h"

#define LEAF        0
#define BRANCH      1
#define EXTENSION   2

#define NL(POINTER) ((evm_node_leaf_p)(POINTER))
#define NB(POINTER) ((evm_node_branch_p)(POINTER))
#define NE(POINTER) ((evm_node_extension_p)(POINTER))

STRUCT(evm_node)
{
    uint64_t type;
};

STRUCT(evm_node_leaf)
{
    evm_node_t n;
    byte_vec_t path;
    byte_vec_t v;
};

STRUCT(evm_node_branch)
{
    evm_node_t n;
    word_t next[16];
    byte_vec_t v;
};

STRUCT(evm_node_extension)
{
    evm_node_t n;
    byte_vec_t path;
    word_t next;
};

#endif
