/*
 * parser.c
 */

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <gmp.h>
#include <mpfr.h>
#include "../include/lib.h"
#include "../include/buffer.h"

int parser_integerp(char *str) {
    if (*str == '+' || *str == '-') str++;
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int parser_ratiop(char *str) {
    if (!strchr(str, '/')) return 0;
    if (*str == '+' || *str == '-') str++;
    if (*str == '/') return 0;
    while (*str != '/') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    str++;
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int parser_floatp(char *str) {
    int no_numerators;
    if (!strchr(str, '.')) return 0;
    if (*str == '+' || *str == '-') str++;
    no_numerators = (*str == '.');
    while (*str != '.') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    str++;
    if (no_numerators && *str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

void *parser_parse(BUFFER buf) {
    size_t len = buffer_get_size(buf) + 1;
    char *str = (char *)malloc(len);
    buffer_copy(buf, str);
    str[len - 1] = '\0';
    void *retval = 0;
    if (parser_integerp(str)) {
        DATA_INTEGER *integer =
            (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
        integer->h.data_type = DATA_TYPE_INTEGER;
        assert(mpz_init_set_str(integer->z, str, 10) == 0);
        retval = (void *)integer;
    } else if (parser_ratiop(str)) {
        mpq_t q;
        mpq_init(q);
        assert(mpq_set_str(q, str, 10) == 0);
        mpq_canonicalize(q);
        if (mpz_cmp_si(mpq_denref(q), 1) == 0) {
            DATA_INTEGER *integer =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            integer->h.data_type = DATA_TYPE_INTEGER;
            mpz_init_set(integer->z, mpq_numref(q));
            mpq_clear(q);
            retval = (void *)integer;
        } else {
            DATA_RATIO *ratio =
                (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            ratio->h.data_type = DATA_TYPE_RATIO;
            ratio->q[0] = q[0];
            retval = (void *)ratio;
        }
    } else if (parser_floatp(str)) {
        DATA_FLOAT *float1 =
            (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        assert(mpfr_init_set_str(float1->f, str, 10, MPFR_RNDN) == 0);
        retval = (void *)float1;
    } else {
        retval = make_symbol(str);
    }
    free(str);
    return retval;
}

