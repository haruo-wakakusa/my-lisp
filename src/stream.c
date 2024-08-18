/*
 * stream.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <editline.h>
#include <string.h>

int repl_mode = 0;
int ungetc_buffering = 0;
char ungetc_char = '\0';
char *editline_buffer = 0;
char *editline_curpos = "";
char *editline_prompt = "";
FILE *file_stream;

void stream_init_repl(char *prompt) {
    repl_mode = 1;
    editline_prompt = prompt;
}

void stream_init_file(FILE *stream) {
    repl_mode = 0;
    file_stream = stream;
}

void stream_ungetc(char c) {
    ungetc_buffering = 1;
    ungetc_char = c;
}

int stream_getchar() {
    if (!repl_mode) {
        return fgetc(file_stream);
    }
    if (ungetc_buffering) {
        ungetc_buffering = 0;
        return ungetc_char;
    }
    if (!editline_buffer) {
        editline_buffer = readline(editline_prompt);
        editline_curpos = editline_buffer;
        if (!editline_buffer) {
            return EOF;
        }
        add_history(editline_buffer);
    }
    if (*editline_curpos == '\0') {
        free(editline_buffer);
        editline_buffer = 0;
        editline_curpos = "";
        return '\n';
    } else {
        char retval = *editline_curpos;
        editline_curpos++;
        return retval;
    }
}
     
