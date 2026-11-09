#ifndef GREPSH_LEXER_H
#define GREPSH_LEXER_H

#include <stddef.h>

typedef struct lexer {
    const char *current;    // current token being scanned
    const char *start;      // beginning of current token
    size_t line;            // line number
} lexer_t;

void lexer_init(lexer_t *l, const char *src);

const char lexer_consume(lexer_t *l);

const char lexer_peek(lexer_t *l);

#endif
