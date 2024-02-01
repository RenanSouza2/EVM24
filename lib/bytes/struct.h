#ifndef __BYTES_STRUCT_H__
#define __BYTES_STRUCT_H__

#include "header.h"

typedef unsigned char uchar;

STRUCT(bytes)
{
    uchar *v;
    int size;
};

#endif
