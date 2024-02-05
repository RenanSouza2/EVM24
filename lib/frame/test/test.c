#include <string.h>

#include "../debug.h"
#include "../../../utils/clu/bin/header.h"
#include "../../stack/list/struct.h"
#include "../../word/debug.h"



void test_frame_init()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed("0x");
    assert(f.pc == 0);
    assert(f.code.size == 0);
    assert(f.code.v == NULL);
    assert(f.s.count == 0);
    assert(f.s.sl == NULL);
    assert(f.m.size == 0);
    assert(f.m.v == NULL);

    f = frame_init_immed("0xff");
    assert(f.pc == 0);
    assert(f.code.size == 1);
    assert(f.code.v != NULL);
    assert(f.code.v[0] == 0xff);
    assert(f.s.count == 0);
    assert(f.s.sl == NULL);
    assert(f.m.size == 0);
    assert(f.m.v == NULL);
    frame_free(f);

    assert(mem_empty());
}



void test_frame_stop()
{
    printf("\n\t%s\t\t", __func__);

    frame_t f = frame_init_immed("0x");
    frame_o_t fo = frame_stop(&f);
    assert(fo.returndata.size == 0);
    assert(fo.returndata.v == NULL);

    assert(mem_empty());
}

void test_frame_push()
{
    printf("\n\t%s\t\t", __func__);

    for(int i=0; i<=32; i++)
    {
        char str[70];
        memset(str, 0, 70);
        sprintf(str, "0x%x", 0x5f + i);
        for(int j=4; j<4+2*i; j++)
            str[j] = 'f';

        word_t w = word_from_zero();
        memset(w.v, 0xff, i);
        
        frame_t f = frame_init_immed(str);
        assert(frame_push(&f) == true);
        assert(f.pc == i + 1);
        assert(f.s.count == 1);
        assert(f.s.sl != NULL);
        assert(word_eq_bool(&f.s.sl->w, &w) == true);
        frame_free(f);
    }

    frame_t f = frame_init_immed("0x5f");
    for(int i=0; i<1024; i++)
    {
        f.pc = 0;
        assert(frame_push(&f) == true);
    }
    f.pc = 0;
    assert(frame_push(&f) == false);
    frame_free(f);

    assert(mem_empty());
}



void test_frame()
{
    printf("\n%s\t\t", __func__);

    test_frame_init();

    test_frame_stop();
    test_frame_push();

    assert(mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_frame();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
