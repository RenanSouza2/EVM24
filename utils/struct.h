#ifndef STRUCT

#define STRUCT(NAME)                            \
    typedef struct _##NAME NAME##_t;            \
    typedef struct _##NAME * NAME##_p;          \
    struct _##NAME

#define PLACEHOLDER(NAME) typedef struct _##NAME * NAME##_p;

typedef void * handler_p;

#endif
