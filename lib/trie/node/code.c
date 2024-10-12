#include <stdio.h>
#include <stdlib.h>



#include "debug.h"
#include "../../../utils/assert.h"

#ifdef DEBUG

#include "../../../utils/clu/bin/header.h"

#endif

evm_rlp_t rlp_init_node_leaf(evm_node_leaf_p n)
{
    evm_rlp_vec_t r_vec = rlp_vec_init(2);
    r_vec.arr[0] = rlp_init_byte_vec(n->path);
    r_vec.arr[1] = rlp_init_byte_vec(n->v);
    return rlp_init_list(r_vec);
}

evm_rlp_t rlp_init_node(evm_node_p n)
{
    switch (n->type)
    {
        case LEAF: return rlp_init_node_leaf(NL(n));
        case BRANCH: break;
        case EXTENSION: break;
    }
    assert(false);
}

word_t node_encode_header(evm_node_p n);

byte_vec_t node_encode(evm_node_p n)
{
    evm_rlp_t r = rlp_init_node(n);
    return rlp_encode(&r);
}
