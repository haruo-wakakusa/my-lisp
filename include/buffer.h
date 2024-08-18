/*
 * buffer.h
 */

#ifndef BUFFER_H_
#define BUFFER_H_

typedef void * BUFFER;

BUFFER buffer_allocate();
void buffer_free(BUFFER buf);
void buffer_write_char(BUFFER buf, char c);
size_t buffer_get_size(BUFFER buf);
void buffer_copy(BUFFER buf, char *dst);
void buffer_clear(BUFFER buf);

#endif

