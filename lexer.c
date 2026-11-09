#include "lexer.h"

void lexer_init(lexer_t *l, const char *src) {
    l->start = src;
    l->current = src;
    l->line = 1;
}

const char lexer_consume(lexer_t *l) {
    l->current += 1;
    return *(l->current - 1);
}

const char lexer_peek(lexer_t *l) {
    return *(l->current);
}

