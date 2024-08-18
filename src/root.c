/*
 * root.c
 */

#include "../include/type.h"
#include "../include/lib.h"

DATA_ROOT * make_root() {
    DATA_ROOT *r = (DATA_ROOT *)gc_allocate(sizeof(DATA_ROOT));
    r->h.data_type = DATA_TYPE_ROOT;
    return r;
}

