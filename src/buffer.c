/*
 * buffer.c
 */

#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"

#define BUFFER_SIZE 256

typedef struct tag_NODE {
    struct tag_NODE *prev;
    struct tag_NODE *next;
    char body[BUFFER_SIZE];
} NODE;

typedef struct tag_HEADER {
    NODE *start;
    NODE *cur;
    int index;
} HEADER;

BUFFER buffer_allocate() {
    HEADER *h = (HEADER *)malloc(sizeof(HEADER));
    NODE *n = (NODE *)malloc(sizeof(NODE));
    h->start = n;
    h->cur = n;
    h->index = 0;
    n->prev = 0;
    n->next = 0;
    memset(n->body, 0, BUFFER_SIZE);
    return (void *)h;
}

void buffer_free(BUFFER buf) {
    HEADER *h = (HEADER *)buf;
    NODE *n = h->start;
    while (n->next) {
        n = n->next;
        free(n->prev);
    }
    free(n);
    free(h);
}

void buffer_write_char(BUFFER buf, char c) {
    HEADER *h = (HEADER *)buf;
    h->cur->body[h->index] = c;
    h->index++;
    if (h->index == BUFFER_SIZE) {
        NODE *n = (NODE *)malloc(sizeof(NODE));
        h->cur->next = n;
        h->index = 0;
        n->prev = h->cur;
        n->next = 0;
        h->cur = n;
        memset(n->body, 0, BUFFER_SIZE);
    }
}

size_t buffer_get_size(BUFFER buf) {
    HEADER *h = (HEADER *)buf;
    NODE *n = h->start;
    size_t res = 0;
    while (n != h->cur) {
        res += BUFFER_SIZE;
        n = n->next;
    }
    res += h->index;
    return res;
}

void buffer_copy(BUFFER buf, char *dst) {
    HEADER *h = (HEADER *)buf;
    NODE *n = h->start;
    while (n != h->cur) {
        memcpy(dst, n->body, BUFFER_SIZE);
        dst += BUFFER_SIZE;
        n = n->next;
    }
    memcpy(dst, n->body, h->index);
}

void buffer_clear(BUFFER buf) {
    HEADER *h = (HEADER *)buf;
    NODE *n = h->start;
    while (n) {
        memset(n->body, 0, BUFFER_SIZE);
        n = n->next;
    }
    h->cur = h->start;
    h->index = 0;
}

