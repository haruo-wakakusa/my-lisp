/*
 * eval.c
 */

#include <string.h>
#include "../include/type.h"
#include "../include/lib.h"
#include "../include/environment.h"

static void *eval_list(void *obj, void *func_env, void *var_env);
static void *eval_symbol(void *obj, void *func_env, void *var_env);

void *env_func_global = 0;
void *env_var_global = 0;

void eval_init() {
    env_func_global = environment_init(0);
    env_var_global = environment_init(0);
}

void *eval_top_level(void *obj) {
    return eval(obj, env_func_global, env_var_global);
}

void *eval(void *obj, void *func_env, void *var_env) {
    DATA_HEADER *h;

    if (!obj) return obj;
    h = (DATA_HEADER *)obj;
    switch (h->data_type) {
    case DATA_TYPE_CONS:
        return eval_list(obj, func_env, var_env);
    case DATA_TYPE_SYMBOL:
        return eval_symbol(obj, func_env, var_env);
    default:
        return obj;
    }
}

static void *eval_list(void *obj, void *func_env, void *var_env) {
    void *car1 = car(obj);
    DATA_SYMBOL *symbol;
    char *sym;
    if (!listp(obj)) {
        fprintf(stderr, "プロパーなリストではありません\n");
        return 0;
    }
    if (!symbolp(car1)) {
        fprintf(stderr, "不正な関数です\n");
        return 0;
    }
    symbol = (DATA_SYMBOL *)car1;
    sym = get_symbol_string(symbol);
    if (strcmp(sym, "+") == 0) {
        void *args = cdr(obj);
        void *p = args;
        while (p) {
            rplaca(p, eval(car(p), func_env, var_env));
            p = cdr(p);
        }
        return arithmetic_add(args);
    } else if (strcmp(sym, "-") == 0) {
        void *args = cdr(obj);
        void *p = args;
        while (p) {
            rplaca(p, eval(car(p), func_env, var_env));
            p = cdr(p);
        }
        return arithmetic_sub(args);
    } else if (strcmp(sym, "*") == 0) {
        void *args = cdr(obj);
        void *p = args;
        while (p) {
            rplaca(p, eval(car(p), func_env, var_env));
            p = cdr(p);
        }
        return arithmetic_mult(args);
    } else if (strcmp(sym, "/") == 0) {
        void *args = cdr(obj);
        void *p = args;
        while (p) {
            rplaca(p, eval(car(p), func_env, var_env));
            p = cdr(p);
        }
        return arithmetic_div(args);
    } else if (strcmp(sym, "DEFVAR") == 0) {
        if (list_length(obj) != 3) {
            fprintf(stderr, "DEFVARの引数の数が2ではありません\n");
            return 0;
        }
        void *newvar = car(cdr(obj));
        if (!symbolp(newvar)) {
            fprintf(stderr, "DEFVARの引数1がシンボルではありません\n");
            return 0;
        }
        void *newval = car(cdr(cdr(obj)));
        newval = eval(newval, func_env, var_env);
        if (environment_exists(env_var_global, newvar)) {
            fprintf(stderr, "変数%sは既に宣言されています\n", get_symbol_string(newvar));
            return 0;
        }
        environment_add(env_var_global, newvar, newval);
        return newvar;
    } else if (strcmp(sym, "SETQ") == 0) {
        if (list_length(obj) != 3) {
            fprintf(stderr, "SETQの引数の数が2ではありません\n");
            return 0;
        }
        void *var = car(cdr(obj));
        if (!symbolp(var)) {
            fprintf(stderr, "SETQの引数1がシンボルではありません\n");
            return 0;
        }
        void *val = car(cdr(cdr(obj)));
        val = eval(val, func_env, var_env);
        if (!environment_exists_recurse(var_env, var)) {
            fprintf(stderr, "変数%sは宣言されていません\n", get_symbol_string(var));
            return 0;
        }
        environment_modify_recurse(var_env, var, val);
        return val;
    } else if (strcmp(sym, "LET") == 0) {
        if (list_length(obj) < 2) {
            fprintf(stderr, "LETの引数が少なすぎます\n");
            return 0;
        }
        void *new_var_env = environment_init(var_env);
        void *bindings = car(cdr(obj));
        if (!listp(bindings)) {
            fprintf(stderr, "LETの引数1が束縛リストではありません\n");
            return 0;
        }
        if (list_length(bindings) == 0) {
            fprintf(stderr, "LETの束縛リストが空です\n");
            return 0;
        }
        while (bindings) {
            void *binding = car(bindings);
            if (!listp(binding) || list_length(binding) != 2) {
                fprintf(stderr, "LETの束縛リストが不正な形式です\n");
                return 0;
            }
            void *var = car(binding);
            if (!symbolp(var)) {
                fprintf(stderr, "LETの束縛リストが不正な形式です\n");
                return 0;
            }
            void *val = car(cdr(binding));
            val = eval(val, func_env, var_env);
            environment_add(new_var_env, var, val);
            bindings = cdr(bindings);
        }
        void *retval = 0;
        void *p = cdr(cdr(obj));
        while (p) {
            retval = eval(car(p), func_env, new_var_env);
            p = cdr(p);
        }
        return retval;
    } else if (strcmp(sym, "IF") == 0) {
        if (list_length(obj) < 3 || list_length(obj) > 4) {
            fprintf(stderr, "IFの引数の数が多すぎるか少なすぎます\n");
            return 0;
        }
        void *test = eval(car(cdr(obj)), func_env, var_env);
        if (test) {
            return eval(car(cdr(cdr(obj))), func_env, var_env);
        } else {
            return eval(car(cdr(cdr(cdr(obj)))), func_env, var_env);
        }
    } else if (strcmp(sym, "DEFUN") == 0) {
        if (list_length(obj) < 3) {
            fprintf(stderr, "DEFUNの引数の数が少なすぎます\n");
            return 0;
        }
        void *funcname = car(cdr(obj));
        if (!symbolp(funcname)) {
            fprintf(stderr, "DEFUNの引数1がシンボルではありません\n");
            return 0;
        }
        void *arglist = car(cdr(cdr(obj)));
        void *p = arglist;
        while (p) {
            if (!symbolp(car(p))) {
                fprintf(stderr, "DEFUNの引数リストの中身がシンボルではありません\n");
                return 0;
            }
            p = cdr(p);
        }
        DATA_USER_DEFINED_FUNCTION *f =
            (DATA_USER_DEFINED_FUNCTION *)
                gc_allocate(sizeof(DATA_USER_DEFINED_FUNCTION));
        f->h.data_type = DATA_TYPE_USER_DEFINED_FUNCTION;
        f->f = cdr(cdr(obj));
        f->func_env = func_env;
        f->var_env = var_env;
        if (environment_exists(env_func_global, funcname)) {
            environment_modify(env_func_global, funcname, f);
        } else {
            environment_add(env_func_global, funcname, f);
        }
        return funcname;
    } else {
        void *f;
        if (environment_get_recurse(func_env, symbol, &f)) {
            DATA_HEADER *h = (DATA_HEADER *)f;
            if (h->data_type == DATA_TYPE_BUILT_IN_FUNCTION) {
                fprintf(stderr, "未実装のコードです\n");
                return 0;
            } else if (h->data_type == DATA_TYPE_USER_DEFINED_FUNCTION) {
                DATA_USER_DEFINED_FUNCTION *func =
                    (DATA_USER_DEFINED_FUNCTION *)f;
                if (list_length(obj) - 1 != list_length(car(func->f))) {
                    fprintf(stderr, "関数の引数の数が一致しません\n");
                    return 0;
                }
                void *args1 = car(func->f);
                void *args2 = cdr(obj);
                void *p = args2;
                while (p) {
                    rplaca(p, eval(car(p), func_env, var_env));
                    p = cdr(p);
                }
                void *new_var_env = environment_init(func->var_env);
                while (args1) {
                    environment_add(new_var_env, car(args1), car(args2));
                    args1 = cdr(args1);
                    args2 = cdr(args2);
                }
                void *retval = 0;
                void *body = cdr(func->f);
                while (body) {
                    retval = eval(car(body), func->func_env, new_var_env);
                    body = cdr(body);
                }
                return retval;
            } else {
                fprintf(stderr, "未実装のコードに到達しました\n");
                return 0;
            }
        } else {
            fprintf(stderr, "%sという名前の関数がありません\n", sym);
            return 0;
        }
    }
}

static void *eval_symbol(void *obj, void *func_env, void *var_env) {
    DATA_SYMBOL *symbol = (DATA_SYMBOL *)obj;
    char *sym = get_symbol_string(symbol);
    void *retval = 0;
    if (strcmp(sym, "NIL") == 0) {
        return 0;
    } else if (strcmp(sym, "T") == 0) {
        return T;
    } else if (environment_get_recurse(var_env, symbol, &retval)) {
        return retval;
    } else {
        fprintf(stderr, "%sという名前の変数がありません\n", sym);
        return 0;
    }
}

