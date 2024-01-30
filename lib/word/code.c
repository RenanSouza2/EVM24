#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

void hello_debug()
{
    printf("\n\nThis function helps to debug");
    printf("\nbut is not compiled in the final build");
    printf("\n");
}

#endif

void hello_example()
{
    printf("\nHello example library");
}


handler_p hello_malloc()
{
    printf("\n\nHello malloc");

    return malloc(4);
}

