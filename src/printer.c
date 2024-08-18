/*
 * printer.c
 */

#include <stdio.h>
#include "../include/type.h"
#include "../include/lib.h"

static void print_symbol(FILE *stream, void *obj);
static void print_list(FILE *stream, void *obj);
static void print_string(FILE *stream, void *obj);
static void print_integer(FILE *stream, void *obj);
static void print_ratio(FILE *stream, void *obj);
static void print_float(FILE *stream, void *obj);
static void print_t(FILE *stream, void *obj);

void printer_print(FILE *stream, void *obj) {
    DATA_HEADER *h;

    if (!obj) {
        fprintf(stream, "NIL");
        return;
    }
    h = (DATA_HEADER *)obj;
    switch (h->data_type) {
    case DATA_TYPE_SYMBOL:
        print_symbol(stream, obj);
        break;
    case DATA_TYPE_CONS:
        print_list(stream, obj);
        break;
    case DATA_TYPE_STRING:
        print_string(stream, obj);
        break;
    case DATA_TYPE_INTEGER:
        print_integer(stream, obj);
        break;
    case DATA_TYPE_RATIO:
        print_ratio(stream, obj);
        break;
    case DATA_TYPE_FLOAT:
        print_float(stream, obj);
        break;
    case DATA_TYPE_T:
        print_t(stream, obj);
        break;
    default:
        fprintf(stderr, "未実装のコードに到達しました\n");
    }
}

static void print_symbol(FILE *stream, void *obj) {
    DATA_SYMBOL *symbol = (DATA_SYMBOL *)obj;
    fprintf(stream, "%s", get_symbol_string(symbol));
}

static void print_list(FILE *stream, void *obj) {
    DATA_CONS *cons = (DATA_CONS *)obj;
    fputc('(', stream);
    printer_print(stream, cons->car);
    obj = cons->cdr;
    while (1) {
        if (!obj) {
            break;
        } else if (!consp(obj)) {
            fprintf(stream, " . ");
            printer_print(stream, obj);
            break;
        }
        cons = (DATA_CONS *)obj;
        fputc(' ', stream);
        printer_print(stream, cons->car);
        obj = cons->cdr;
    }
    fputc(')', stream);
}

static void print_string(FILE *stream, void *obj) {
    DATA_STRING *string = (DATA_STRING *)obj;
    fputc('"', stream);
    fprintf(stream, "%s", get_string_string(string));
    fputc('"', stream);
}

static void print_integer(FILE *stream, void *obj) {
    DATA_INTEGER *integer = (DATA_INTEGER *)obj;
    mpz_out_str(stream, 10, integer->z);
}

static void print_ratio(FILE *stream, void *obj) {
    DATA_RATIO *ratio = (DATA_RATIO *)obj;
    mpq_out_str(stream, 10, ratio->q);
}

static void print_float(FILE *stream, void *obj) {
    DATA_FLOAT *float1 = (DATA_FLOAT *)obj;
    mpfr_out_str(stream, 10, 10, float1->f, MPFR_RNDN);
}

static void print_t(FILE *stream, void *obj) {
    fputc('T', stream);
}
