#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "tokens.h"

const char lexer_consume(lexer_t *l) {
    l->current += 1;
    return *(l->current - 1);
}

const char lexer_peek(lexer_t *l) {
    return *(l->current);
}

token_t token_make(lexer_t *l, token_type_t t) {
    return (struct token) {
        .start = l->start,
        .type = t,
        .len = l->current - l->start
    };
}

token_t token_next(lexer_t *l) {
    // skip whitespace
    while (1) {
        char c = lexer_peek(l);
        if (c == ' ' || c == '\r' || c == '\t') {
            lexer_consume(l);
        } else if (c == '\n') {
            l->line += 1;
            lexer_consume(l);
        } else {
            break;
        }
    }

    l->start = l->current;
    char c = lexer_consume(l);

    // single-character tokens
    switch (c) {
        case ';': return token_make(l, TOKEN_SEMICOLON);
        case '=':
            if (lexer_peek(l) != ' ') {
                c = lexer_consume(l);
                switch (c) {
                    case '=': return token_make(l, TOKEN_EQ);
                    case '>': return token_make(l, TOKEN_MATCH_ARM);
                    case '~': return token_make(l, TOKEN_REGEX_EQ);
                }
            }
            return token_make(l, TOKEN_ASSIGN);
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            if (lexer_peek(l) == '=') {
                lexer_consume(l);
                switch (c) {
                    case '+': return token_make(l, TOKEN_ADD_ASSIGN);
                    case '-': return token_make(l, TOKEN_SUB_ASSIGN);
                    case '*': return token_make(l, TOKEN_MUL_ASSIGN);
                    case '/': return token_make(l, TOKEN_DIV_ASSIGN);
                    case '%': return token_make(l, TOKEN_MOD_ASSIGN);
                }
            }
            switch (c) {
                case '+': return token_make(l, TOKEN_ADD);
                case '-': return token_make(l, TOKEN_SUB);
                case '*': return token_make(l, TOKEN_MUL);
                case '/': return token_make(l, TOKEN_DIV);
                case '%': return token_make(l, TOKEN_MOD);
            }
        case '!':
            if (lexer_peek(l) != ' ') {
                c = lexer_consume(l);
                switch (c) {
                    case '=': return token_make(l, TOKEN_NEQ);
                    case '~': return token_make(l, TOKEN_REGEX_NEQ);
                }
            }
            return token_make(l, TOKEN_NOT);
        case '>':
        case '<':
            if (lexer_peek(l) == '=') {
                lexer_consume(l);
                switch (c) {
                    case '>': return token_make(l, TOKEN_GEQ);
                    case '<': return token_make(l, TOKEN_LEQ);
                }
            }
            switch (c) {
                case '>': return token_make(l, TOKEN_GT);
                case '<': return token_make(l, TOKEN_LT);
            }
        case '@': return token_make(l, TOKEN_MATCH_WHOLE);
        case '{': return token_make(l, TOKEN_LBRACE);
        case '}': return token_make(l, TOKEN_RBRACE);
        case '[': return token_make(l, TOKEN_LBRACKET);
        case ']': return token_make(l, TOKEN_RBRACKET);
        case '(': return token_make(l, TOKEN_LPAREN);
        case ')': return token_make(l, TOKEN_RPAREN);
        case ',': return token_make(l, TOKEN_COMMA);
        case '.': return token_make(l, TOKEN_DOT);
        case ':':
            if (lexer_peek(l) == ':') {
                lexer_consume(l);
                return token_make(l, TOKEN_MODULE_ACCESSOR);
            }
            return token_make(l, TOKEN_COLON);
    }

    // identifiers/keywords
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        while (
            (lexer_peek(l) >= 'a' && lexer_peek(l) <= 'z') ||
            (lexer_peek(l) >= 'A' && lexer_peek(l) <= 'Z') ||
            (lexer_peek(l) >= '0' && lexer_peek(l) <= '9') || 
            lexer_peek(l) == '_'
        ) lexer_consume(l);

        const struct {
            const char *s;          // string representation
            const token_type_t t;   // token mapping
        } tokens[] = {
            {"let", TOKEN_LET},
            {"mut", TOKEN_MUT},
            {"fn", TOKEN_FN},
            {"struct", TOKEN_STRUCT},
            {"match", TOKEN_MATCH},
            {"replace", TOKEN_REPLACE},
            {"if", TOKEN_IF},
            {"else", TOKEN_ELSE},
            {"for", TOKEN_FOR},
            {"in", TOKEN_IN},
            {"while", TOKEN_WHILE},
            {"return", TOKEN_RETURN},
            {"break", TOKEN_BREAK},
            {"continue", TOKEN_CONTINUE},
            {"as", TOKEN_AS},
            {"true", TOKEN_TRUE},
            {"false", TOKEN_FALSE},

            {"Int", TOKEN_INT},
            {"Float", TOKEN_FLOAT},
            {"String", TOKEN_STRING},
            {"Bool", TOKEN_BOOL},
            {"Regex", TOKEN_REGEX},
            {"Array", TOKEN_ARRAY},
            {"Map", TOKEN_MAP}
        };
        const size_t tokens_len = sizeof(tokens) / sizeof(tokens[0]);

        size_t token_len = l->current - l->start;
        for (size_t i = 0; i < tokens_len; i++) {
            if (strlen(tokens[i].s) != token_len) continue;
            if (memcmp(l->start, tokens[i].s, token_len) == 0)
                return token_make(l, tokens[i].t);
        }
        return token_make(l, TOKEN_IDENTIFIER);
    }

    // char
    if (c == '\'') {
        while (lexer_peek(l) != '\'') lexer_consume(l);
        lexer_consume(l);
        return token_make(l, TOKEN_CHAR);
    }

    // string/interpolated string/regex
    if (c == '"' || c == 'f' || c == 'r') {
        while (lexer_peek(l) != '"') lexer_consume(l);
        lexer_consume(l);
        switch (c) {
            case '"': return token_make(l, TOKEN_STRING);
            case 'f': return token_make(l, TOKEN_INTERPOLATED_STRING);
            case 'r': return token_make(l, TOKEN_REGEX);
        }
    }

    return token_make(l, TOKEN_EOF);
}

int main() {
    const char *src = "let text = \"hello\";\nprint(text);";

    struct lexer l;
    lexer_init(&l, src);

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


}
