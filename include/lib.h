/*
 * lib.h
 */

#ifndef LIB_H_
#define LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include "../include/type.h"
#include "../include/buffer.h"

/* gc.c */

void *gc_allocate(size_t size);
void gc_initialize();
void gc_mark(void *roots);
void gc_sweep();
int get_gc_stack_length();

/* root.c */

DATA_ROOT * make_root();

/* symbol.c */

DATA_SYMBOL * make_symbol(char *str1);
DATA_SYMBOL * make_symbol_from_buffer(BUFFER buf);
char * get_symbol_string(DATA_SYMBOL *symbol);
int symbolp(void *obj);

/* string.h */

DATA_STRING * make_string(char *str1);
DATA_STRING * make_string_from_buffer(BUFFER buf);
char * get_string_string(DATA_STRING *string);

/* cons.c */

DATA_CONS * make_cons(void *car1, void *cdr1);
int consp(void *obj);
void *car(void *obj);
void *cdr(void *obj);
void *rplaca(void *cons, void *obj);
void *rplacd(void *cons, void *obj);
int listp(void *obj);
size_t list_length(void *obj);

/* reader.c */

void reader_initialize();
void *reader_read();

/* printer.c */

void printer_print(FILE *stream, void *obj);

/* dot.c */

void fix_to_dotted_list(void *obj);
int check_single_dot(void *obj);

/* parser.c */

void *parser_parse(BUFFER buf);

/* eval.c */

void eval_init();
void *eval_top_level(void *obj);
void *eval(void *obj, void *func_env, void *var_env);

extern void *env_func_global;
extern void *env_var_global;

/* arithmetic.c */

void *arithmetic_add(void *args);
void *arithmetic_sub(void *args);
void *arithmetic_mult(void *args);
void *arithmetic_div(void *args);
int realp(void *obj);
int floatp(void *obj);

#endif
