#ifndef __NODE_STRUCT_H__
#define __NODE_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"

#define LEAF        0
#define BRANCH      1
#define EXTENSION   2

STRUCT(evm_node)
{
    uint64_t type;
};

STRUCT(evm_node_leaf)
{
    byte_vec_p path;
    byte_vec_p val;
};

// STRUCT(evm_node_branch)
// {
//     evm_word_t
// }

#endif
