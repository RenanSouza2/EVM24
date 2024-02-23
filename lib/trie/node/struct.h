#ifndef __NODE_STRUCT_H__
#define __NODE_STRUCT_H__

#include "header.h"

#include "../../utils/struct.h"

#define LEAF        0
#define BRANCH      1
#define EXTENSION   2

STRUCT(evm_node)
{
    int type;
};

STRUCT(evm_node_leaf)
{
    byte_p path;
    byte_p val;
};

#endif
