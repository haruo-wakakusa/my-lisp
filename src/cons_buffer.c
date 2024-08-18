/*
 * cons_buffer.c
 */

#include <stdlib.h>
#include "../include/cons_buffer.h"
#include "../include/type.h"
#include "../include/lib.h"

typedef struct tag_HEADER {
    DATA_CONS *top;
    DATA_CONS *last;
} HEADER;

CONS_BUFFER cons_buffer_allocate() {
    HEADER *h = (HEADER *)malloc(sizeof(HEADER));
    h->top = 0;
    h->last = 0;
    return (void *)h;
}

void cons_buffer_add(CONS_BUFFER buf, void *obj) {
    HEADER *h = (HEADER *)buf;
    if (!h->top) {
        h->top = make_cons(obj, 0);
        h->last = h->top;
    } else {
        h->last->cdr = make_cons(obj, 0);
        h->last = h->last->cdr;
    }
}

void *cons_buffer_get_list(CONS_BUFFER buf) {
    return (void *) ((HEADER *)buf)->top;
}

void cons_buffer_free(CONS_BUFFER buf) {
    free(buf);
}

