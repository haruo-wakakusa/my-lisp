/* test_editline.c */

#include <stdio.h>
#include <stdlib.h>
#include <editline.h>
#include <string.h>

int main() {
    char *str;

    while (1) {
        str = readline("* ");
        if (strcmp(str, "exit") == 0) break;
        add_history(str);
        printf("output: %s\n", str);
        free(str);
    }

    return 0;
}

