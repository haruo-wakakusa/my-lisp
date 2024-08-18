/*
 * test_reader.c
 */

#include <stdio.h>
#include "../include/lib.h"
#include "../include/stream.h"

int main() {
    void *obj;
    stream_init_repl("* ");
    reader_initialize();
    while (1) {
        obj = reader_read();
        printer_print(stdout, obj);
        putchar('\n');
    }
    return 0;
}

