/*
 * dot.c
 */

#include <string.h>
#include "../include/type.h"
#include "../include/lib.h"

/*
 * この関数は、リーダーで得られたオブジェクトに対して適用するので、
 * リストはドッティドリストを含まないことを想定している。
 */
void fix_to_dotted_list(void *obj) {
    DATA_HEADER *h = (DATA_HEADER *)obj;
    if (h->data_type == DATA_TYPE_CONS) {
        /* リスト上の要素全てに再帰を適用する */
        {
            void *p = obj;
            while (p) {
                DATA_CONS *q = (DATA_CONS *)p;
                fix_to_dotted_list(q->car);
                p = q->cdr;
            }
        }
        /* リストの長さを数える */
        {
            size_t len = 0;
            void *p = obj;
            DATA_CONS *q;
            void *r;
            DATA_CONS *s;
            void *t;
            DATA_CONS *u;
            while (p) {
                len++;
                p = ((DATA_CONS *)p)->cdr;
            }
            /* 最後から３つ目に移動 */
            if (len < 3) return;
            p = obj;
            for (len -= 3; len != 0; len--) {
                p = ((DATA_CONS *)p)->cdr;
            }
            /* 最後から２つ目がドット(.)のシンボルであるかを調べる */
            q = (DATA_CONS *)p;
            r = q->cdr;
            s = (DATA_CONS *)r;
            t = s->cdr;
            u = (DATA_CONS *)t;
            if (((DATA_HEADER *)(s->car))->data_type != DATA_TYPE_SYMBOL)
                return;
            if (strcmp(get_symbol_string((DATA_SYMBOL *)(s->car)), ".") != 0)
                return;
            /* 最後の要素を３番目のcdrに付け替える */
            q->cdr = u->car;
        }
    }
}

/*
 * 非0ならばOK、0ならばNG
 */
int check_single_dot(void *obj) {
    DATA_HEADER *h = (DATA_HEADER *)obj;
    if (h->data_type == DATA_TYPE_SYMBOL) {
        DATA_SYMBOL *s = (DATA_SYMBOL *)obj;
        return strcmp(get_symbol_string(s), ".");
    }
    if (consp(obj)) {
        void *p = obj;
        while (consp(p)) {
            if (!check_single_dot(car(p))) return 0;
            p = cdr(p);
        }
        if (p && !check_single_dot(p)) return 0;
    }
    return 1;
}

