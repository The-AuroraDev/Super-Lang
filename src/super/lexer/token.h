#ifndef SUPER_TOKEN_H
#define SUPER_TOKEN_H

#include <stddef.h>

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENT,
    TOKEN_INT_LIT,
    TOKEN_FLOAT_LIT,
    TOKEN_STRING_LIT,
    TOKEN_CHAR_LIT,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NIL,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_LONGINT,
    TOKEN_BOOL,
    TOKEN_VEC,
    TOKEN_FN,
    TOKEN_STRUCT,
    TOKEN_INTERFACE,
    TOKEN_TYPE,
    TOKEN_PUB,
    TOKEN_INCLUDE,
    TOKEN_MAIN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_SWITCH,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_RETURN,
    TOKEN_VAR,
    TOKEN_CONST,
    TOKEN_MUT,
    TOKEN_AS,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_ARROW,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_ELLIPSIS,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_POW,
    TOKEN_PLUS_ASSIGN,
    TOKEN_MINUS_ASSIGN,
    TOKEN_STAR_ASSIGN,
    TOKEN_SLASH_ASSIGN,
    TOKEN_PERCENT_ASSIGN,
    TOKEN_POW_ASSIGN,
    TOKEN_INC,
    TOKEN_DEC,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_LE,
    TOKEN_GT,
    TOKEN_GE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_BIT_AND,
    TOKEN_BIT_OR,
    TOKEN_BIT_XOR,
    TOKEN_BIT_NOT,
    TOKEN_LSHIFT,
    TOKEN_RSHIFT,
    TOKEN_AMPERSAND,
    TOKEN_QUESTION,
    TOKEN_HASH,
    TOKEN_GC,
    TOKEN_SAY,
    TOKEN_LISTEN,
    TOKEN_IN,
    TOKEN_OUT,
    TOKEN_ERROR,
} TokenKind;

typedef struct {
    TokenKind kind;
    const char *start;
    size_t length;
    size_t line;
    size_t column;
    size_t offset;
} Token;

const char *token_kind_name(TokenKind kind);
void token_print(Token token);

#endif