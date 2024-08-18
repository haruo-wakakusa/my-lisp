/* test_buffer.c */

#include <stdio.h>
#include <assert.h>
#include "../include/buffer.h"

int main() {
    BUFFER buffer = buffer_allocate();
    assert(buffer_get_size(buffer) == 0);
    buffer_write_char(buffer, 'a');
    assert(buffer_get_size(buffer) == 1);
    buffer_clear(buffer);
    /*
    assert(buffer_get_size(buffer) == 0);
    */
    buffer_free(buffer);

    return 0;
}

