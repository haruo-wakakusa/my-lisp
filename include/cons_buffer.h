/*
 * cons_buffer.h
 */

#ifndef CONS_BUFFER_H_
#define CONS_BUFFER_H_

typedef void * CONS_BUFFER;

CONS_BUFFER cons_buffer_allocate();
void cons_buffer_add(CONS_BUFFER buf, void *obj);
void *cons_buffer_get_list(CONS_BUFFER buf);
void cons_buffer_free(CONS_BUFFER buf);

#endif

