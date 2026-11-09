#include <stdio.h>
#include "print.h"

void print_source_trace(const char *source_buf, const struct token *tokens,
                        size_t token_count) {
    printf("==================================================\n\n");

    size_t current_line = 1;
    const char *line_start = source_buf;
    size_t token_idx = 0;

    for (size_t i = 0; source_buf[i] != '\0'; i++) {
        if (source_buf[i] == '\n' || source_buf[i + 1] == '\0') {
            size_t line_len = &source_buf[i] - line_start + (source_buf[i] == '\n' ? 0 : 1);
            
            printf("[Line %zu] %.*s\n", current_line, (int)line_len, line_start);

            size_t line_token_count = 0;
            size_t temp_idx = token_idx;
            while (temp_idx < token_count && tokens[temp_idx].line == current_line) {
                if (tokens[temp_idx].type != TOKEN_EOF) {
                    line_token_count++;
                }
                temp_idx++;
            }

            size_t printed = 0;
            while (token_idx < token_count && tokens[token_idx].line == current_line) {
                if (tokens[token_idx].type == TOKEN_EOF) {
                    token_idx++;
                    continue;
                }

                printed++;
                const char *connector = (printed == line_token_count) ? "└──" : "├──";

                printf("         %s %-18s (\"%.*s\")\n",
                    connector,
                    token_to_str(tokens[token_idx].type),
                    (int)tokens[token_idx].len,
                    tokens[token_idx].start
                );
                token_idx++;
            }

            printf("\n");
            current_line++;
            line_start = &source_buf[i + 1];
        }
    }
}
