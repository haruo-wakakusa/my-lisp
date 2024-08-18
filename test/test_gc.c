/*
 * test_gc.c
 */

#include <stdio.h>
#include "../include/type.h"
#include "../include/lib.h"

int main() {
    DATA_ROOT *root = make_root();
    DATA_SYMBOL *s1 = make_symbol("ABC");
    DATA_SYMBOL *s2 = make_symbol("DEF");
    DATA_SYMBOL *s3 = make_symbol("XYZ");
    DATA_CONS *a = make_cons((void *)s2, 0);
    DATA_CONS *b = make_cons((void *)s1, (void *)a);
    DATA_CONS *c = make_cons((void *)s3, (void *)b);
    root->bind = b;
    root->gc_tmp = 0;
    printf("gc stack length: %d\n", get_gc_stack_length());
    gc_initialize();
    printf("gc_initialize() ... ok\n");
    gc_mark((void *)root);
    printf("gc_mark() ... ok\n");
    gc_sweep();
    printf("gc_sweep() ... ok\n");
    printf("gc stack length: %d\n", get_gc_stack_length());

    return 0;
}

