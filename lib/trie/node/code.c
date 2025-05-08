#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../../mods/clu/header.h"
#include "../../../mods/macros/assert.h"

#include "../../rlp/header.h"
#include "../../word/header.h"



#ifdef DEBUG
#endif



// TODO test
evm_rlp_t rlp_init_node_leaf(evm_node_leaf_p n)
{
    evm_rlp_vec_t r_vec = rlp_vec_init(2);
    r_vec.arr[0] = rlp_init_byte_vec(n->path);
    r_vec.arr[1] = rlp_init_byte_vec(n->v);
    return rlp_init_list(r_vec);
}

// TODO test
evm_rlp_t rlp_init_node_branch(evm_node_branch_p n)
{
    evm_rlp_vec_t r_vec = rlp_vec_init(17);
    for(uint64_t i = 0; i < 16; i++)
    {
        byte_vec_t v = byte_vec_init_word(&n->next[i]);
        r_vec.arr[i] = rlp_init_byte_vec(v);
    }
    r_vec.arr[16] = rlp_init_byte_vec(n->v);
    return rlp_init_list(r_vec);
}

// TODO test
evm_rlp_t rlp_init_node_extension(evm_node_extension_p n)
{
    evm_rlp_vec_t r_vec = rlp_vec_init(2);
    r_vec.arr[0] = rlp_init_byte_vec(n->path);

    byte_vec_t v = byte_vec_init_word(&n->next);
    r_vec.arr[1] = rlp_init_byte_vec(v);

    return rlp_init_list(r_vec);
}

// TODO test
evm_rlp_t rlp_init_node(evm_node_p n)
{
    switch (n->type)
    {
    case LEAF:
        return rlp_init_node_leaf(NL(n));
    case BRANCH:
        return rlp_init_node_branch(NB(n));
    case EXTENSION:
        return rlp_init_node_extension(NE(n));
    }
    assert(false);
}

word_t node_encode_header(evm_node_p n);

// TODO test
byte_vec_t node_encode(evm_node_p n)
{
    evm_rlp_t r = rlp_init_node(n);
    return rlp_encode(&r);
}
