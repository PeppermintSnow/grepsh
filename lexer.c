#include "lexer.h"

void lexer_init(lexer_t *l, const char *src) {
    l->start = src;
    l->current = src;
    l->line = 1;
}
