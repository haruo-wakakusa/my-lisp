/* test_dot.c */

#include "../include/lib.h"

int main() {
    void *obj;
    reader_initialize();
    while (1) {
        printf("* ");
        obj = reader_read(stdin);
        fix_to_dotted_list(obj);
        printf("check_single_dot() = %d\n", check_single_dot(obj));
        printer_print(stdout, obj);
        putchar('\n');
    }
    return 0;
}
