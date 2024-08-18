/*
 * reader.c
 */

#include <stdio.h>
#include <ctype.h>
#include "../include/type.h"
#include "../include/lib.h"
#include "../include/buffer.h"
#include "../include/cons_buffer.h"
#include "../include/stream.h"

BUFFER buffer;

static void *read_list();
static void *read_string();

void reader_initialize() {
    buffer = buffer_allocate();
}

void *reader_read() {
    while (1) {
        int c;
LOOP1:
        c = stream_getchar();
        if (c == EOF) {
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                return retval;
            } else {
                exit(0); /* EOFに達したのでプログラムを終了 */
            }
        }
        switch ((char)c) {
        case '(':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                return read_list();
            }
            break;
        case ')':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                fprintf(stderr, "余分な閉じ括弧です\n");
                return 0;
            }
            break;
        case '"':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                return read_string();
            }
            break;
        case ';':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                while (1) {
                    c = stream_getchar();
                    if (c == EOF) {
                        exit(0); /* EOFに達したのでプログラムを終了 */
                    } else if (c == '\n') {
                        goto LOOP1;
                    }
                }
            }
            break;
        case '#':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                c = stream_getchar();
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    return 0;
                }
                switch (c) {
                case '\'':
                    return make_cons(make_symbol("FUNCTION"),
                                     make_cons(reader_read(), 0));
                default:
                    fprintf(stderr, "未実装のマクロ文字です\n");
                    return 0;
                }
            }
            break;
        case '\'':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            } else {
                return make_cons(make_symbol("QUOTE"),
                    make_cons(reader_read(), 0));
            }
            break;
        case ' ':
        case '\n':
        case '\t':
            if (buffer_get_size(buffer) != 0) {
                void *retval = parser_parse(buffer);
                buffer_clear(buffer);
                stream_ungetc(c);
                return retval;
            }
            break;
        default:
            buffer_write_char(buffer, toupper(c));
        }
    }
}

static void *read_list() {
    CONS_BUFFER cbuf = cons_buffer_allocate();
    while (1) {
        int c;
LOOP1:
        c = stream_getchar();
        if (c == EOF) {
            fprintf(stderr, "ファイルの終わりに到達しました\n");
            cons_buffer_free(cbuf);
            return 0;
        }
        switch ((char)c) {
        case '(':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            cons_buffer_add(cbuf, read_list());
            break;
        case ')':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            {
                void *retval = cons_buffer_get_list(cbuf);
                cons_buffer_free(cbuf);
                return retval;
            }
            break;
        case '"':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            {
                void *str = read_string();
                if (!str) {
                    cons_buffer_free(cbuf);
                    return 0;
                }
                cons_buffer_add(cbuf, str);
            }
            break;
        case ';':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            while (1) {
                c = stream_getchar();
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    cons_buffer_free(cbuf);
                    return 0;
                } else if (c == '\n') {
                    goto LOOP1;
                }
            }
            break;
        case '#':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            c = stream_getchar();
            if (c == EOF) {
                fprintf(stderr, "ファイルの終わりに到達しました\n");
                cons_buffer_free(cbuf);
                return 0;
            }
            switch (c) {
            case '\'':
                cons_buffer_add(cbuf,
                    make_cons(make_symbol("FUNCTION"),
                        make_cons(reader_read(), 0)));
                break;
            default:
                fprintf(stderr, "未実装のマクロ文字です");
                cons_buffer_free(cbuf);
                return 0;
            }
            break;
        case '\'':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            } else {
                cons_buffer_add(cbuf,
                    make_cons(make_symbol("QUOTE"),
                        make_cons(reader_read(), 0)));
            }
            break;
        case ' ':
        case '\n':
        case '\t':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, parser_parse(buffer));
                buffer_clear(buffer);
            }
            break;
        default:
            buffer_write_char(buffer, toupper((char)c));
        }
    }
}

static void *read_string() {
    while (1) {
        int c = stream_getchar();
        if (c == EOF) {
            fprintf(stderr, "ファイルの終わりに到達しました\n");
            return 0;
        }
        switch ((char)c) {
        case '"':
            {
                void *retval = make_string_from_buffer(buffer);
                buffer_clear(buffer);
                return retval;
            }
            break;
        default:
            buffer_write_char(buffer, c);
        }
    }
}

