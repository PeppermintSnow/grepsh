#ifndef GREPSH_TOKENS_H
#define GREPSH_TOKENS_H

#include <stddef.h>

#include "lexer.h"

typedef enum token_type {
    // declarators
    TOKEN_LET,
    TOKEN_MUT,
    TOKEN_FN,
    TOKEN_STRUCT,

    // types
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_REGEX,
    TOKEN_BOOL,
    TOKEN_ARRAY,
    TOKEN_MAP,

    // literals
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_INTERPOLATED_STRING,
    TOKEN_REGEX_LITERAL,

    // assignment operators
    TOKEN_ASSIGN,
    TOKEN_ADD_ASSIGN,
    TOKEN_SUB_ASSIGN,
    TOKEN_MUL_ASSIGN,
    TOKEN_DIV_ASSIGN,
    TOKEN_MOD_ASSIGN,

    // arithmetic operators
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,

    // relation operators
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LEQ,
    TOKEN_GEQ,
    TOKEN_REGEX_EQ,
    TOKEN_REGEX_NEQ,

    // logical operators
    TOKEN_NOT,
    TOKEN_AND,
    TOKEN_OR,

    // unary operators
    TOKEN_POS,
    TOKEN_NEG,

    // match operators
    TOKEN_MATCH_ARM,
    TOKEN_MATCH_WHOLE,

    // control flow
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_IN,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_MATCH,
    TOKEN_REPLACE,

    // typecasting
    TOKEN_AS,

    // boolean literal
    TOKEN_TRUE,
    TOKEN_FALSE,

    // comments
    TOKEN_COMMENT,

    // delimiters
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MODULE_ACCESSOR,
    TOKEN_COLON,
    TOKEN_ARROW,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,

    // error/eof
    TOKEN_ERROR,
    TOKEN_EOF
} token_type_t;

typedef struct token {
    token_type_t type;  // token type
    const char *start;  // ptr to starting pos of the token
    size_t len;         // token length
} token_t;

token_t token_make(lexer_t *l, token_type_t t);

token_t token_next(lexer_t *l);

const char *token_to_str(const token_type_t t);

#endif
