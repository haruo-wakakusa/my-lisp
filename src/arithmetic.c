/*
 * arithmetic.c
 */

#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include "../include/type.h"
#include "../include/lib.h"

static void *arithmetic_div2(void *op1, void *op2);
static void *arithmetic_one();
static void *float_(void *obj);
static void *integer_to_ratio(void *obj);

void *arithmetic_add(void *args) {
    int has_float = 0;
    void *p = args;
    while (p) {
        DATA_HEADER *h;
        void *car1 = car(p);
        if (!car1) {
            fprintf(stderr, "function + error: NILは引数にできません\n");
        }
        h = (DATA_HEADER *)car1;
        switch (h->data_type) {
        case DATA_TYPE_INTEGER:
        case DATA_TYPE_RATIO:
            break;
        case DATA_TYPE_FLOAT:
            has_float = 1;
            break;
        default:
            fprintf(stderr, "function + error: not arithmetic type\n");
            return 0;
        }
        p = cdr(p);
    }

    if (has_float) {
        mpfr_t acc;
        mpfr_init(acc);
        mpfr_set_si(acc, 0, MPFR_RNDN);
        void *p = args;
        DATA_FLOAT *float1;
        while (p) {
            void *car1 = car(p);
            DATA_HEADER *h = (DATA_HEADER *)car1;
            switch (h->data_type) {
            case DATA_TYPE_INTEGER:
                mpfr_add_z(acc, acc, ((DATA_INTEGER *)car1)->z, MPFR_RNDN);
                break;
            case DATA_TYPE_RATIO:
                mpfr_add_q(acc, acc, ((DATA_RATIO *)car1)->q, MPFR_RNDN);
                break;
            case DATA_TYPE_FLOAT:
                mpfr_add(acc, acc, ((DATA_FLOAT *)car1)->f, MPFR_RNDN);
                break;
            default:
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        float1 = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        mpfr_init(float1->f);
        mpfr_swap(float1->f, acc);
        mpfr_clear(acc);
        return (void *)float1;
    } else {
        mpq_t acc;
        mpq_t op;
        mpq_init(acc);
        mpq_init(op);
        void *p = args;
        while (p) {
            void *car1 = car(p);
            DATA_HEADER *h = (DATA_HEADER *)car1;
            switch (h->data_type) {
            case DATA_TYPE_INTEGER:
                mpz_set(mpq_numref(op), ((DATA_INTEGER *)car1)->z);
                mpq_add(acc, acc, op);
                break;
            case DATA_TYPE_RATIO:
                mpq_add(acc, acc, ((DATA_RATIO *)car1)->q);
                break;
            default:
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        mpq_canonicalize(acc);
        if (mpz_cmp(mpq_denref(acc), mpq_denref(op)) == 0) {
            DATA_INTEGER *integer =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            integer->h.data_type = DATA_TYPE_INTEGER;
            mpz_init_set(integer->z, mpq_numref(acc));
            mpq_clear(op);
            mpq_clear(acc);
            return (void *)integer;
        } else {
            DATA_RATIO *ratio =
                (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            ratio->h.data_type = DATA_TYPE_RATIO;
            mpq_init(ratio->q);
            mpq_swap(ratio->q, acc);
            mpq_clear(acc);
            mpq_clear(op);
            return (void *)ratio;
        }
    }
}

void *arithmetic_sub(void *args) {
    int has_float = 0;
    void *p = args;
    void *car1;
    size_t len_args = list_length(args);

    if (len_args == 0) {
        fprintf(stderr, "function - error: 引数が0個で呼び出されました\n");
        return 0;
    }
    if (len_args == 1) {
        DATA_HEADER *h;
        car1 = car(args);
        if (!car1) {
            fprintf(stderr, "function - error: 引数にNILが渡されました\n");
            return 0;
        }
        h = (DATA_HEADER *)car1;
        if (h->data_type == DATA_TYPE_INTEGER) {
            DATA_INTEGER *integer = (DATA_INTEGER *)car1;
            DATA_INTEGER *retval =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            retval->h.data_type = DATA_TYPE_INTEGER;
            mpz_init(retval->z);
            mpz_neg(retval->z, integer->z);
            return (void *)retval;
        } else if (h->data_type == DATA_TYPE_RATIO) {
            DATA_RATIO *ratio = (DATA_RATIO *)car1;
            DATA_RATIO *retval =
                (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            retval->h.data_type = DATA_TYPE_RATIO;
            mpq_init(retval->q);
            mpq_neg(retval->q, ratio->q);
            return (void *)retval;
        } else if (h->data_type == DATA_TYPE_FLOAT) {
            DATA_FLOAT *float1 = (DATA_FLOAT *)car1;
            DATA_FLOAT *retval =
                (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
            retval->h.data_type = DATA_TYPE_FLOAT;
            mpfr_init(retval->f);
            mpfr_neg(retval->f, float1->f, MPFR_RNDN);
            return (void *)retval;
        } else {
            fprintf(stderr, "function - error: 引数が数値型でありません\n");
            return 0;
        }
    }

    while (p) {
        DATA_HEADER *h;
        car1 = car(p);
        if (!car1) {
            fprintf(stderr, "function - error: 引数にNILが渡されました\n");
            return 0;
        }
        h = (DATA_HEADER *)car1;
        switch (h->data_type) {
        case DATA_TYPE_INTEGER:
        case DATA_TYPE_RATIO:
            break;
        case DATA_TYPE_FLOAT:
            has_float = 1;
            break;
        default:
            fprintf(stderr, "function - error: 引数が数値型でありません\n");
            return 0;
        }
        p = cdr(p);
    }

    if (has_float) {
        mpfr_t acc;
        DATA_HEADER *h;
        p = args;
        car1 = car(p);
        h = (DATA_HEADER *)car1;
        if (h->data_type == DATA_TYPE_INTEGER) {
            DATA_INTEGER *integer = (DATA_INTEGER *)car1;
            mpfr_init_set_z(acc, integer->z, MPFR_RNDN);
        } else if (h->data_type == DATA_TYPE_RATIO) {
            DATA_RATIO *ratio = (DATA_RATIO *)car1;
            mpfr_init_set_q(acc, ratio->q, MPFR_RNDN);
        } else if (h->data_type == DATA_TYPE_FLOAT) {
            DATA_FLOAT *float1 = (DATA_FLOAT *)car1;
            mpfr_init_set(acc, float1->f, MPFR_RNDN);
        } else {
            fprintf(stderr, "未実装コードに到達しました\n");
        }
        p = cdr(p);
        while (p) {
            car1 = car(p);
            h = (DATA_HEADER *)car1;
            if (h->data_type == DATA_TYPE_INTEGER) {
                DATA_INTEGER *integer = (DATA_INTEGER *)car1;
                mpfr_sub_z(acc, acc, integer->z, MPFR_RNDN);
            } else if (h->data_type == DATA_TYPE_RATIO) {
                DATA_RATIO *ratio = (DATA_RATIO *)car1;
                mpfr_sub_q(acc, acc, ratio->q, MPFR_RNDN);
            } else if (h->data_type == DATA_TYPE_FLOAT) {
                DATA_FLOAT *float1 = (DATA_FLOAT *)car1;
                mpfr_sub(acc, acc, float1->f, MPFR_RNDN);
            } else {
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        {
            DATA_FLOAT *retval = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
            retval->h.data_type = DATA_TYPE_FLOAT;
            mpfr_init(retval->f);
            mpfr_swap(retval->f, acc);
            mpfr_clear(acc);
            return (void *)retval;
        }
    } else {
        mpq_t acc;
        mpq_t op;
        mpq_init(acc);
        mpq_init(op);
        DATA_HEADER *h;
        p = args;
        car1 = car(p);
        h = (DATA_HEADER *)car1;
        if (h->data_type == DATA_TYPE_INTEGER) {
            DATA_INTEGER *integer = (DATA_INTEGER *)car1;
            mpq_set_z(acc, integer->z);
        } else if (h->data_type == DATA_TYPE_RATIO) {
            DATA_RATIO *ratio = (DATA_RATIO *)car1;
            mpq_set(acc, ratio->q);
        } else {
            fprintf(stderr, "未実装コードに到達しました\n");
        }
        p = cdr(p);
        while (p) {
            car1 = car(p);
            h = (DATA_HEADER *)car1;
            if (h->data_type == DATA_TYPE_INTEGER) {
                DATA_INTEGER *integer = (DATA_INTEGER *)car1;
                mpz_set(mpq_numref(op), integer->z);
                mpq_sub(acc, acc, op);
            } else if (h->data_type == DATA_TYPE_RATIO) {
                DATA_RATIO *ratio = (DATA_RATIO *)car1;
                mpq_sub(acc, acc, ratio->q);
            } else {
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        mpq_canonicalize(acc);
        if (mpz_cmp(mpq_denref(acc), mpq_denref(op)) == 0) {
            DATA_INTEGER *integer =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            integer->h.data_type = DATA_TYPE_INTEGER;
            mpz_init_set(integer->z, mpq_numref(acc));
            mpq_clear(acc);
            mpq_clear(op);
            return (void *)integer;
        } else {
            DATA_RATIO *ratio = (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            ratio->h.data_type = DATA_TYPE_RATIO;
            mpq_init(ratio->q);
            mpq_swap(ratio->q, acc);
            mpq_clear(acc);
            mpq_clear(op);
            return (void *)ratio;
        }
    }
}

void *arithmetic_mult(void *args) {
    int has_float = 0;
    void *p = args;
    while (p) {
        DATA_HEADER *h;
        void *car1 = car(p);
        if (!car1) {
            fprintf(stderr, "function * error: NILは引数にできません\n");
        }
        h = (DATA_HEADER *)car1;
        switch (h->data_type) {
        case DATA_TYPE_INTEGER:
        case DATA_TYPE_RATIO:
            break;
        case DATA_TYPE_FLOAT:
            has_float = 1;
            break;
        default:
            fprintf(stderr, "function * error: 数値型ではありません\n");
            return 0;
        }
        p = cdr(p);
    }

    if (has_float) {
        mpfr_t acc;
        mpfr_init(acc);
        mpfr_set_si(acc, 1, MPFR_RNDN);
        void *p = args;
        DATA_FLOAT *float1;
        while (p) {
            void *car1 = car(p);
            DATA_HEADER *h = (DATA_HEADER *)car1;
            switch (h->data_type) {
            case DATA_TYPE_INTEGER:
                mpfr_mul_z(acc, acc, ((DATA_INTEGER *)car1)->z, MPFR_RNDN);
                break;
            case DATA_TYPE_RATIO:
                mpfr_mul_q(acc, acc, ((DATA_RATIO *)car1)->q, MPFR_RNDN);
                break;
            case DATA_TYPE_FLOAT:
                mpfr_mul(acc, acc, ((DATA_FLOAT *)car1)->f, MPFR_RNDN);
                break;
            default:
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        float1 = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        mpfr_init(float1->f);
        mpfr_swap(float1->f, acc);
        mpfr_clear(acc);
        return (void *)float1;
    } else {
        mpq_t acc;
        mpq_t op;
        mpq_init(acc);
        mpq_set_si(acc, 1, 1);
        mpq_init(op);
        void *p = args;
        while (p) {
            void *car1 = car(p);
            DATA_HEADER *h = (DATA_HEADER *)car1;
            switch (h->data_type) {
            case DATA_TYPE_INTEGER:
                mpz_set(mpq_numref(op), ((DATA_INTEGER *)car1)->z);
                mpq_mul(acc, acc, op);
                break;
            case DATA_TYPE_RATIO:
                mpq_mul(acc, acc, ((DATA_RATIO *)car1)->q);
                break;
            default:
                fprintf(stderr, "未実装コードに到達しました\n");
            }
            p = cdr(p);
        }
        mpq_canonicalize(acc);
        if (mpz_cmp(mpq_denref(acc), mpq_denref(op)) == 0) {
            DATA_INTEGER *integer =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            integer->h.data_type = DATA_TYPE_INTEGER;
            mpz_init_set(integer->z, mpq_numref(acc));
            mpq_clear(op);
            mpq_clear(acc);
            return (void *)integer;
        } else {
            DATA_RATIO *ratio = (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            ratio->h.data_type = DATA_TYPE_RATIO;
            mpq_init(ratio->q);
            mpq_swap(ratio->q, acc);
            mpq_clear(acc);
            mpq_clear(op);
            return (void *)ratio;
        }
    }
}

void *arithmetic_div(void *args) {
    size_t len_args = list_length(args);
    void *acc;

    if (len_args == 0) {
        fprintf(stderr, "function / error: 引数が0個で呼び出されました\n");
        return 0;
    }
    if (len_args == 1) {
        return arithmetic_div2(arithmetic_one(), car(args));
    }
    acc = car(args);
    args = cdr(args);
    while (args) {
        acc = arithmetic_div2(acc, car(args));
        args = cdr(args);
    }
    return acc;
}

static void *arithmetic_div2(void *op1, void *op2) {
    if (!realp(op1) || !realp(op2)) {
        fprintf(stderr, "function / error: 引数が数値でありません\n");
        return 0;
    }
    if (floatp(op1) || floatp(op2)) {
        DATA_FLOAT *float1 = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        op1 = float_(op1);
        op2 = float_(op2);
        mpfr_init(float1->f);
        mpfr_div(float1->f, ((DATA_FLOAT *)op1)->f, ((DATA_FLOAT *)op2)->f, MPFR_RNDN);
        if (mpfr_nan_p(float1->f) || mpfr_inf_p(float1->f)) {
            fprintf(stderr, "function / error: ゼロ割りが行われました\n");
            return 0;
        }
        return (void *)float1;
    } else {
        mpq_t rop;
        op1 = integer_to_ratio(op1);
        op2 = integer_to_ratio(op2);
        if (mpz_cmp_si(mpq_numref(((DATA_RATIO *)op2)->q), 0) == 0) {
            fprintf(stderr, "function / error: ゼロ割りが行われました\n");
            return 0;
        }
        mpq_init(rop);
        mpq_div(rop, ((DATA_RATIO *)op1)->q, ((DATA_RATIO *)op2)->q);
        mpq_canonicalize(rop);
        if (mpz_cmp_si(mpq_denref(rop), 1) == 0) {
            DATA_INTEGER *integer =
                (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
            integer->h.data_type = DATA_TYPE_INTEGER;
            mpz_init_set(integer->z, mpq_numref(rop));
            mpq_clear(rop);
            return (void *)integer;
        } else {
            DATA_RATIO *ratio = (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
            ratio->h.data_type = DATA_TYPE_RATIO;
            mpq_init(ratio->q);
            mpq_swap(ratio->q, rop);
            mpq_clear(rop);
            return (void *)ratio;
        }
    }
}

static void *arithmetic_one() {
    DATA_INTEGER *integer = (DATA_INTEGER *)gc_allocate(sizeof(DATA_INTEGER));
    integer->h.data_type = DATA_TYPE_INTEGER;
    mpz_init_set_si(integer->z, 1);
    return (void *)integer;
}

int realp(void *obj) {
    DATA_HEADER *h;
    if (!obj) return 0;
    h = (DATA_HEADER *)obj;
    switch (h->data_type) {
    case DATA_TYPE_INTEGER:
    case DATA_TYPE_RATIO:
    case DATA_TYPE_FLOAT:
        return 1;
    default:
        return 0;
    }
}

int floatp(void *obj) {
    if (!obj) return 0;
    return ((DATA_HEADER *)obj)->data_type == DATA_TYPE_FLOAT;
}

static void *float_(void *obj) {
    DATA_HEADER *h;
    if (!obj) {
        fprintf(stderr, "NILはREAL型ではありません\n");
    }
    h = (DATA_HEADER *)obj;
    if (h->data_type == DATA_TYPE_INTEGER) {
        DATA_INTEGER *integer = (DATA_INTEGER *)obj;
        DATA_FLOAT *float1 = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        mpfr_init_set_z(float1->f, integer->z, MPFR_RNDN);
        return (void *)float1;
    } else if (h->data_type == DATA_TYPE_RATIO) {
        DATA_RATIO *ratio = (DATA_RATIO *)obj;
        DATA_FLOAT *float1 = (DATA_FLOAT *)gc_allocate(sizeof(DATA_FLOAT));
        float1->h.data_type = DATA_TYPE_FLOAT;
        mpfr_init_set_q(float1->f, ratio->q, MPFR_RNDN);
        return (void *)float1;
    } else if (h->data_type == DATA_TYPE_FLOAT) {
        return obj;
    } else {
        fprintf(stderr, "REAL型ではありません\n");
        return 0;
    }
}

static void *integer_to_ratio(void *obj) {
    DATA_HEADER *h;
    if (!obj) {
        fprintf(stderr, "NILは有理数型ではありません\n");
        return 0;
    }
    h = (DATA_HEADER *)obj;
    if (h->data_type == DATA_TYPE_INTEGER) {
        DATA_INTEGER *integer = (DATA_INTEGER *)obj;
        DATA_RATIO *ratio = (DATA_RATIO *)gc_allocate(sizeof(DATA_RATIO));
        mpq_init(ratio->q);
        mpq_set_z(ratio->q, integer->z);
        return (void *)ratio;
    } else if (h->data_type == DATA_TYPE_RATIO) {
        return obj;
    } else {
        fprintf(stderr, "有理数型ではありません\n");
        return 0;
    }
}

