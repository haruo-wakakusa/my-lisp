/*
 * symbol.c
 */

#include <string.h>
#include "../include/type.h"
#include "../include/lib.h"
#include "../include/buffer.h"

DATA_SYMBOL * make_symbol(char *str1) {
    size_t len;
    DATA_SYMBOL *symbol;
    char *s;

    len = strlen(str1) + 1;
    symbol = gc_allocate(sizeof(DATA_SYMBOL) + len);
    symbol->h.data_type = DATA_TYPE_SYMBOL;
    symbol->size = len;
    s = (char *)symbol;
    strcpy(s + sizeof(DATA_SYMBOL), str1);

    return symbol;
}

DATA_SYMBOL * make_symbol_from_buffer(BUFFER buf) {
    size_t len;
    DATA_SYMBOL *symbol;
    char *s;

    len = buffer_get_size(buf) + 1;
    symbol = gc_allocate(sizeof(DATA_SYMBOL) + len);
    symbol->h.data_type = DATA_TYPE_SYMBOL;
    symbol->size = len;
    s = (char *)symbol;
    buffer_copy(buf, s + sizeof(DATA_SYMBOL));
    s[sizeof(DATA_SYMBOL) + len - 1] = '\0';

    return symbol;
}

char * get_symbol_string(DATA_SYMBOL *symbol) {
    char *s;

    s = (char *)symbol;
    return s + sizeof(DATA_SYMBOL);
}

int symbolp(void *obj) {
    DATA_HEADER *h = (DATA_HEADER *)obj;
    return (h->data_type == DATA_TYPE_SYMBOL);
}

