/*
 * type.h
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <gmp.h>
#include <mpfr.h>

typedef enum tag_DATA_TYPE {
    DATA_TYPE_ROOT,
    DATA_TYPE_SYMBOL,
    DATA_TYPE_CONS,
    DATA_TYPE_STRING,
    DATA_TYPE_INTEGER,
    DATA_TYPE_RATIO,
    DATA_TYPE_FLOAT,
    DATA_TYPE_T,
    DATA_TYPE_BUILT_IN_FUNCTION,
    DATA_TYPE_USER_DEFINED_FUNCTION
} DATA_TYPE;

typedef struct tag_DATA_HEADER {
    void *gc_next;
    int gc_marker;
    DATA_TYPE data_type;
} DATA_HEADER;

typedef struct tag_DATA_ROOT {
    DATA_HEADER h;
    void *bind;
    void *gc_tmp;
} DATA_ROOT;

typedef struct tag_DATA_SYMBOL {
    DATA_HEADER h;
    int size;
} DATA_SYMBOL;

typedef struct tag_DATA_STRING {
    DATA_HEADER h;
    int size;
} DATA_STRING;

typedef struct tag_DATA_CONS {
    DATA_HEADER h;
    void *car;
    void *cdr;
    void *gc_tmp;
} DATA_CONS;

typedef struct tag_DATA_INTEGER {
    DATA_HEADER h;
    mpz_t z;
} DATA_INTEGER;

typedef struct tag_DATA_RATIO {
    DATA_HEADER h;
    mpq_t q;
} DATA_RATIO;

typedef struct tag_DATA_FLOAT {
    DATA_HEADER h;
    mpfr_t f;
} DATA_FLOAT;

typedef struct tag_DATA_T {
    DATA_HEADER h;
} DATA_T;

typedef struct tag_DATA_BUILT_IN_FUNCTION {
    DATA_HEADER h;
    void *(*f)(void *, void *, void *);
} DATA_BUILT_IN_FUNCTION;

typedef struct tag_DATA_USER_DEFINED_FUNCTION {
    DATA_HEADER h;
    void *f;
    void *func_env;
    void *var_env;
} DATA_USER_DEFINED_FUNCTION;

extern void *T;

#endif

