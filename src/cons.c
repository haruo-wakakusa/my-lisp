/*
 * cons.c
 */

#include "../include/type.h"
#include "../include/lib.h"

DATA_CONS * make_cons(void *car1, void *cdr1) {
    DATA_CONS *c = gc_allocate(sizeof(DATA_CONS));
    c->h.data_type = DATA_TYPE_CONS;
    c->car = car1;
    c->cdr = cdr1;
    return c;
}

int consp(void *obj) {
    DATA_HEADER *h;

    if (!obj) return 0;
    h = (DATA_HEADER *)obj;
    if (h->data_type == DATA_TYPE_CONS) {
        return 1;
    }
    return 0;
}

void *car(void *obj) {
    if (!obj) return 0;
    if (!consp(obj)) {
        fprintf(stderr, "CARはアトムに適用できません\n");
        return 0;
    }
    return ((DATA_CONS *)obj)->car;
}

void *cdr(void *obj) {
    if (!obj) return 0;
    if (!consp(obj)) {
        fprintf(stderr, "CDRはアトムに適用できません\n");
        return 0;
    }
    return ((DATA_CONS *)obj)->cdr;
}

void *rplaca(void *cons, void *obj) {
    if (!consp(cons)) {
        fprintf(stderr, "コンスセルではありません\n");
        return 0;
    }
    ((DATA_CONS *)cons)->car = obj;
    return cons;
}

void *rplacd(void *cons, void *obj) {
    if (!consp(cons)) {
        fprintf(stderr, "コンスセルではありません\n");
        return 0;
    }
    ((DATA_CONS *)cons)->cdr = obj;
    return cons;
}

int listp(void *obj) {
    while (obj) {
        if (!consp(obj)) return 0;
        obj = cdr(obj);
    }
    return 1;
}

size_t list_length(void *obj) {
    size_t len = 0;
    while (obj) {
        if (!consp(obj)) {
            fprintf(stderr, "プロパーなリストではありません\n");
            return -1;
        }
        obj = cdr(obj);
        len++;
    }
    return len;
}

