/* stream.h */

#ifndef STREAM_H_
#define STREAM_H_

void stream_init_repl(char *prompt);
void stream_init_file(FILE *stream);
void stream_ungetc(char c);
int stream_getchar();

#endif

