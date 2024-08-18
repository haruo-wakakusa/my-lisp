/* test_printer.c */

#include "../include/lib.h"

int main() {
    printf("printing string object: ");
    printer_print(stdout, make_string("abc"));
    putchar('\n');
    printf("printing list object: ");
    {
        void *symbol = make_symbol("123");
        void *string = make_string("abc");
        void *cons = make_cons(symbol, make_cons(string, 0));
        printer_print(stdout, cons);
    }
    putchar('\n');
    return 0;
}

