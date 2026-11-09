#include "tokens.h"
#include <string.h>

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
        } else if (c == '#') {
            while (lexer_peek(l) != '\n' && lexer_peek(l) != '\0') 
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
            if (lexer_peek(l) == '=' ||
                lexer_peek(l) == '>' ||
                lexer_peek(l) == '~'
            ) {
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
            if (lexer_peek(l) != '=' || lexer_peek(l) == '~') {
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

    // char
    if (c == '\'') {
        while (lexer_peek(l) != '\'') lexer_consume(l);
        lexer_consume(l);
        return token_make(l, TOKEN_CHAR);
    }

    // string
    if (c == '"') {
        while (lexer_peek(l) != '"' && lexer_peek(l) != '\0') lexer_consume(l);
        lexer_consume(l);
        return token_make(l, TOKEN_STRING_LITERAL);
    }

    // interpolated string/regex
    if ((c == 'f' || c == 'r') && lexer_peek(l) == '"') {
        lexer_consume(l);
        while (lexer_peek(l) != '"' && lexer_peek(l) != '\0') lexer_consume(l);
        lexer_consume(l);
        token_type_t t = c == 'f' ? TOKEN_INTERPOLATED_STRING : TOKEN_REGEX_LITERAL;
        return token_make(l, t);
    }

    if (c >= '0' && c <= '9') {
        while (lexer_peek(l) >= '0' && lexer_peek(l) <= '9') lexer_consume(l);

        if (lexer_peek(l) == '.' ) {
            lexer_consume(l);
            while (lexer_peek(l) >= '0' && lexer_peek(l) <= '9') lexer_consume(l);
            return token_make(l, TOKEN_FLOAT_LITERAL);
        }

        return token_make(l, TOKEN_INT_LITERAL);
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

    return token_make(l, TOKEN_EOF);
}

const char *token_to_str(const token_type_t t) {
    switch (t) {
        // declarators
        case TOKEN_LET: return "LET";
        case TOKEN_MUT: return "MUT";
        case TOKEN_FN: return "FN";
        case TOKEN_STRUCT: return "STRUCT";

        // types
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_REGEX: return "REGEX";
        case TOKEN_BOOL: return "BOOL";
        case TOKEN_ARRAY: return "ARRAY";
        case TOKEN_MAP: return "MAP";

        // literals
        case TOKEN_INT_LITERAL: return "INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_INTERPOLATED_STRING: return "INTERPOLATED_STRING";
        case TOKEN_REGEX_LITERAL: return "REGEX_LITERAL";

        // assignment operators
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_ADD_ASSIGN: return "ADD_ASSIGN";
        case TOKEN_SUB_ASSIGN: return "SUB_ASSIGN";
        case TOKEN_MUL_ASSIGN: return "MUL_ASSIGN";
        case TOKEN_DIV_ASSIGN: return "DIV_ASSIGN";
        case TOKEN_MOD_ASSIGN: return "MOD_ASSIGN";

        // arithmetic operators
        case TOKEN_ADD: return "ADD";
        case TOKEN_SUB: return "SUB";
        case TOKEN_MUL: return "MUL";
        case TOKEN_DIV: return "DIV";
        case TOKEN_MOD: return "MOD";

        // relation operators
        case TOKEN_EQ: return "EQ";
        case TOKEN_NEQ: return "NEQ";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LEQ: return "LEQ";
        case TOKEN_GEQ: return "GEQ";
        case TOKEN_REGEX_EQ: return "REGEX_EQ";
        case TOKEN_REGEX_NEQ: return "REGEX_NEQ";

        // logical operators
        case TOKEN_NOT: return "NOT";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";

        // unary operators
        case TOKEN_POS: return "POS";
        case TOKEN_NEG: return "NEG";

        // match operators
        case TOKEN_MATCH_ARM: return "MATCH_ARM";
        case TOKEN_MATCH_WHOLE: return "MATCH_WHOLE";

        // control flow
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IN: return "IN";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_MATCH: return "MATCH";
        case TOKEN_REPLACE: return "REPLACE";

        // typecasting
        case TOKEN_AS: return "AS";

        // boolean literal
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";

        // comments
        case TOKEN_COMMENT: return "COMMENT";

        // delimiters
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_MODULE_ACCESSOR: return "MODULE_ACCESSOR";
        case TOKEN_COLON: return "COLON";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";

        // error/eof
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_EOF: return "EOF";
    }
}
