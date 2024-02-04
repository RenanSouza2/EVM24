#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../bytes/debug.h"
#include "../../word/debug.h"


void test_get_word()
{
    printf("\n\t%s\t\t", __func__);

    mem_t m = mem_init();
    word_t w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_immed(w, 0, 0, 0, 0));
    
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_immed(w, 0, 0, 0, 0));
    
    bytes_set_byte(&m, 31, 0xff);
    w = mem_get_word(&m, 0);
    assert(m.size == 32);
    assert(w.v);
    assert(word_immed(w, 0, 0, 0, 0xff));
    
    w = mem_get_word(&m, 31);
    assert(m.size == 63);
    assert(w.v);
    assert(word_immed(w, U64_FF, 0, 0, 0));
    
    w = mem_get_word(&m, 32);
    assert(m.size == 64);
    assert(w.v);
    assert(word_immed(w, 0, 0, 0, 0));
    mem_free(m);
    
    assert(mem_empty());
}



void test_mem()
{
    printf("\n%s\t\t", __func__);

    test_get_word();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
