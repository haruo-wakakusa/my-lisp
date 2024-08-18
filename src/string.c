/*
 * string.c
 */

#include <string.h>
#include "../include/type.h"
#include "../include/lib.h"
#include "../include/buffer.h"

DATA_STRING * make_string(char *str1) {
    size_t len;
    DATA_STRING *string;
    char *s;

    len = strlen(str1) + 1;
    string = gc_allocate(sizeof(DATA_STRING) + len);
    string->h.data_type = DATA_TYPE_STRING;
    string->size = len;
    s = (char *)string;
    strcpy(s + sizeof(DATA_STRING), str1);

    return string;
}

DATA_STRING * make_string_from_buffer(BUFFER buf) {
    size_t len;
    DATA_STRING *string;
    char *s;

    len = buffer_get_size(buf) + 1;
    string = gc_allocate(sizeof(DATA_STRING) + len);
    string->h.data_type = DATA_TYPE_STRING;
    string->size = len;
    s = (char *)string;
    buffer_copy(buf, s + sizeof(DATA_STRING));
    s[sizeof(DATA_STRING) + len - 1] = '\0';

    return string;
}

char * get_string_string(DATA_STRING *string) {
    char *s;

    s = (char *)string;
    return s + sizeof(DATA_STRING);
}

