#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "print.h"
#include "tokens.h"

int main(int argc, char *argv[]) {
    if (argc < 1) {
        perror("missing path to grepsh source code");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("error opening file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char *buf = malloc(fsize + 1);
    if (buf == NULL) {
        perror("malloc failed");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t bytes_read = fread(buf, 1, fsize, file);
    if (bytes_read < fsize)
        if (ferror(file))
            perror("error reading file");
    buf[bytes_read] = '\0';
    fclose(file);

    struct lexer l;
    lexer_init(&l, buf);

    size_t token_cap = 128;
    size_t token_idx = 0;
    struct token *tokens = malloc(token_cap * sizeof(struct token));

    struct token t;
    do {
        t = token_next(&l);       
        if (token_idx >= token_cap) {
            token_cap *= 2;
            tokens = realloc(tokens, token_cap * sizeof(struct token));
        }
        tokens[token_idx] = t;
        token_idx += 1;
    } while (t.type != TOKEN_EOF);

    print_source_trace(buf, tokens, token_idx);

    free(tokens);
    free(buf);

    return EXIT_SUCCESS;
}
