/*
 * environment.c
 */

#include <stdlib.h>
#include <string.h>
#include "../include/type.h"
#include "../include/lib.h"

void *environment_init(void *parent) {
    return make_cons(0, parent);
}

void environment_add(void *env, void *key, void *val) {
    void *assoc = make_cons(key, val);
    void *joint = make_cons(assoc, car(env));
    rplaca(env, joint);
}

int environment_exists(void *env, void *key) {
    DATA_SYMBOL *sym1 = (DATA_SYMBOL *)key;
    void *p = car(env);
    while (p) {
        DATA_SYMBOL *sym2 = (DATA_SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_get(void *env, void *key, void **val) {
    DATA_SYMBOL *sym1 = (DATA_SYMBOL *)key;
    void *p = car(env);
    while (p) {
        DATA_SYMBOL *sym2 = (DATA_SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            *val = cdr(car(p));
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_modify(void *env, void *key, void *val) {
    DATA_SYMBOL *sym1 = (DATA_SYMBOL *)key;
    void *p = car(env);
    while (p) {
        DATA_SYMBOL *sym2 = (DATA_SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            rplacd(car(p), val);
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_remove(void *env, void *key) {
    DATA_SYMBOL *sym1 = (DATA_SYMBOL *)key;
    DATA_SYMBOL *sym2;
    void *p = car(env);
    void *q;
    if (!p) return 0;
    sym2 = (DATA_SYMBOL *)car(car(p));
    if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
        rplaca(env, cdr(p));
        return 1;
    }
    q = p;
    p = cdr(p);
    while (p) {
        sym2 = (DATA_SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            rplacd(q, cdr(p));
            return 1;
        }
        q = p;
        p = cdr(p);
    }
    return 0;
}

int environment_get_recurse(void *env, void *key, void **val) {
    while (env) {
        if (environment_get(env, key, val)) return 1;
        env = cdr(env);
    }
    return 0;
}

int environment_exists_recurse(void *env, void *key) {
    while (env) {
        if (environment_exists(env, key)) return 1;
        env = cdr(env);
    }
    return 0;
}

int environment_modify_recurse(void *env, void *key, void *val) {
    while (env) {
        if (environment_modify(env, key, val)) return 1;
        env = cdr(env);
    }
    return 0;
}

