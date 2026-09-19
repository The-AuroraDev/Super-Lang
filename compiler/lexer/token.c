#include "token.h"
#include <stdio.h>
#include <string.h>

const char *token_kind_name(TokenKind kind) {
    switch (kind) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_IDENT: return "IDENT";
        case TOKEN_INT_LIT: return "INT_LIT";
        case TOKEN_FLOAT_LIT: return "FLOAT_LIT";
        case TOKEN_STRING_LIT: return "STRING_LIT";
        case TOKEN_CHAR_LIT: return "CHAR_LIT";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_NIL: return "NIL";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_LONGINT: return "LONGINT";
        case TOKEN_BOOL: return "BOOL";
        case TOKEN_VEC: return "VEC";
        case TOKEN_FN: return "FN";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_INTERFACE: return "INTERFACE";
        case TOKEN_TYPE: return "TYPE";
        case TOKEN_PUB: return "PUB";
        case TOKEN_INCLUDE: return "INCLUDE";
        case TOKEN_MAIN: return "MAIN";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_SWITCH: return "SWITCH";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_VAR: return "VAR";
        case TOKEN_CONST: return "CONST";
        case TOKEN_MUT: return "MUT";
        case TOKEN_AS: return "AS";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COLON: return "COLON";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_ELLIPSIS: return "ELLIPSIS";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PERCENT: return "PERCENT";
        case TOKEN_POW: return "POW";
        case TOKEN_PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TOKEN_MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TOKEN_STAR_ASSIGN: return "STAR_ASSIGN";
        case TOKEN_SLASH_ASSIGN: return "SLASH_ASSIGN";
        case TOKEN_PERCENT_ASSIGN: return "PERCENT_ASSIGN";
        case TOKEN_POW_ASSIGN: return "POW_ASSIGN";
        case TOKEN_INC: return "INC";
        case TOKEN_DEC: return "DEC";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NEQ: return "NEQ";
        case TOKEN_LT: return "LT";
        case TOKEN_LE: return "LE";
        case TOKEN_GT: return "GT";
        case TOKEN_GE: return "GE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_BIT_AND: return "BIT_AND";
        case TOKEN_BIT_OR: return "BIT_OR";
        case TOKEN_BIT_XOR: return "BIT_XOR";
        case TOKEN_BIT_NOT: return "BIT_NOT";
        case TOKEN_LSHIFT: return "LSHIFT";
        case TOKEN_RSHIFT: return "RSHIFT";
        case TOKEN_AMPERSAND: return "AMPERSAND";
        case TOKEN_QUESTION: return "QUESTION";
        case TOKEN_HASH: return "HASH";
        case TOKEN_GC: return "GC";
        case TOKEN_SAY: return "SAY";
        case TOKEN_LISTEN: return "LISTEN";
        case TOKEN_IN: return "IN";
        case TOKEN_OUT: return "OUT";
        case TOKEN_ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void token_print(Token token) {
    printf("%s", token_kind_name(token.kind));
    if (token.length > 0) {
        printf(" \"%.*s\"", (int)token.length, token.start);
    }
    printf(" at %zu:%zu\n", token.line, token.column);
}