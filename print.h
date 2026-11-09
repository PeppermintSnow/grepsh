#ifndef GREPSH_PRINT_H
#define GREPSH_PRINT_H

#include <stddef.h>

#include "tokens.h"

void print_source_trace(const char *source_buf, const struct token *tokens,
                        size_t token_count);

#endif
