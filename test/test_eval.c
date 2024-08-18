/* test_eval.c */

#include <stdio.h>
#include "../include/lib.h"
#include "../include/stream.h"

int main() {
    void *obj;
    stream_init_repl("* ");
    reader_initialize();
    eval_init();
    while (1) {
        obj = reader_read();
        obj = eval_top_level(obj);
        printer_print(stdout, obj);
        putchar('\n');
    }
    return 0;
}
