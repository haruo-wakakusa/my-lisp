/*
 * gc.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/type.h"
#include "../include/lib.h"

static void gc_object_free(void *obj);

void *gc_stack = 0;

void *gc_allocate(size_t size) {
    void *p = malloc(size);
    DATA_HEADER *q;
    if (!p) {
        fprintf(stderr, "メモリの確保に失敗しました");
        exit(1);
    }
    q = (DATA_HEADER *)p;
    q->gc_next = gc_stack;
    gc_stack = p;
    return p;
}

void gc_initialize() {
    void *p = gc_stack;
    DATA_HEADER *q;

    while (p) { /* pがNULLでないならば */
        q = (DATA_HEADER *)p;
        q->gc_marker = 0;
        p = q->gc_next;
    }
}

void gc_mark(void *roots) {

    while (roots) { /* rootsがNULLでないならば */
        void *root = roots;
        DATA_HEADER *root_header = (DATA_HEADER *)root;
        void *p;
        DATA_HEADER *q;

        if (root_header->data_type == DATA_TYPE_CONS) {
            DATA_CONS *root_cons = (DATA_CONS *)root;
            p = root_cons->cdr;
            roots = root_cons->gc_tmp;
        } else if (root_header->data_type == DATA_TYPE_ROOT) {
            DATA_ROOT *root_root = (DATA_ROOT *)root;
            root_root->h.gc_marker = 1;
            p = root_root->bind;
            roots = root_root->gc_tmp;
        } else {
            fprintf(stderr, "未実装コードに到達しました");
            exit(1);
        }

        while (p) { /* pがNULLでないならば */
            q = (DATA_HEADER *)p;
            if (q->gc_marker != 0)
                break; /* 既にマークされている場合はその先に進まない */
            q->gc_marker = 1;
            if (q->data_type == DATA_TYPE_CONS) {
                DATA_CONS *r = (DATA_CONS *)p;
                r->gc_tmp = roots;
                roots = p;
                p = r->car;
            } else {
                break; /* 参照を持たないデータなのでループを抜ける */
            }
        }
    }
}
                
void gc_sweep() {
    void *p;
    DATA_HEADER *q;

    /* リストの最初の要素がマークされていなければスイープ */
    while (gc_stack) { /* gc_stackがNULLでないならば */
        DATA_HEADER *gc_stack_header = (DATA_HEADER *)gc_stack;
        if (gc_stack_header->gc_marker) break;
        void *tmp = gc_stack;
        gc_stack = gc_stack_header->gc_next;
        gc_object_free(tmp);
    }
    
    /* 次の要素からリストを走査する */
    p = gc_stack;
    q = (DATA_HEADER *)p;
    while (p && q->gc_next) {
        void *r = q->gc_next;
        DATA_HEADER *s = (DATA_HEADER *)r;
        if (s->gc_marker == 0) {
            q->gc_next = s->gc_next;
            gc_object_free(r);
        } else {
            p = r;
            q = s;
        }
    }
}

static void gc_object_free(void *obj) {
    DATA_HEADER *h = (DATA_HEADER *)obj;
    switch (h->data_type) {
    case DATA_TYPE_SYMBOL:
    case DATA_TYPE_CONS:
    case DATA_TYPE_STRING:
        break;
    case DATA_TYPE_INTEGER:
        {
            DATA_INTEGER *integer = (DATA_INTEGER *)obj;
            mpz_clear(integer->z);
        }
        break;
    case DATA_TYPE_RATIO:
        {
            DATA_RATIO *ratio = (DATA_RATIO *)obj;
            mpq_clear(ratio->q);
        }
        break;
    case DATA_TYPE_FLOAT:
        {
            DATA_FLOAT *float1 = (DATA_FLOAT *)obj;
            mpfr_clear(float1->f);
        }
        break;
    default:
        ; /* 何もしない */
    }
    free(obj);
}

int get_gc_stack_length() {
    void *p = gc_stack;
    DATA_HEADER *q;
    int count = 0;
    
    while (p) {
        q = (DATA_HEADER *)p;
        p = q->gc_next;
        count++;
    }

    return count;
}

