#include "tokens.h"

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
