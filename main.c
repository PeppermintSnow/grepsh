#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
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

    struct token t;
    while (t.type != TOKEN_EOF) {
        t = token_next(&l);       
        printf("Line %zu [%zu byte%s]: %s\n", 
            l.line, 
            t.len, 
            t.len > 1 ? "s" : "",
            token_to_str(t.type)
        );
    }

    free(buf);
    return EXIT_SUCCESS;
}
