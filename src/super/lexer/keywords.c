#include "super/lexer/keywords.h"
#include <string.h>

static struct {
    const char *word;
    size_t len;
    TokenKind kind;
} keywords[] = {
    {"true", 4, TOKEN_TRUE},
    {"True", 4, TOKEN_TRUE},
    {"false", 5, TOKEN_FALSE},
    {"False", 5, TOKEN_FALSE},
    {"nil", 3, TOKEN_NIL},
    {"int", 3, TOKEN_INT},
    {"float", 5, TOKEN_FLOAT},
    {"string", 6, TOKEN_STRING},
    {"longint", 7, TOKEN_LONGINT},
    {"bool", 4, TOKEN_BOOL},
    {"vec", 3, TOKEN_VEC},
    {"fn", 2, TOKEN_FN},
    {"struct", 6, TOKEN_STRUCT},
    {"interface", 9, TOKEN_INTERFACE},
    {"type", 4, TOKEN_TYPE},
    {"pub", 3, TOKEN_PUB},
    {"include", 7, TOKEN_INCLUDE},
    {"main", 4, TOKEN_MAIN},
    {"if", 2, TOKEN_IF},
    {"else", 4, TOKEN_ELSE},
    {"for", 3, TOKEN_FOR},
    {"switch", 6, TOKEN_SWITCH},
    {"break", 5, TOKEN_BREAK},
    {"continue", 8, TOKEN_CONTINUE},
    {"return", 6, TOKEN_RETURN},
    {"var", 3, TOKEN_VAR},
    {"const", 5, TOKEN_CONST},
    {"mut", 3, TOKEN_MUT},
    {"as", 2, TOKEN_AS},
    {"and", 3, TOKEN_AND},
    {"or", 2, TOKEN_OR},
    {"not", 3, TOKEN_NOT},
    {"say", 3, TOKEN_SAY},
    {"listen", 6, TOKEN_LISTEN},
    {"in", 2, TOKEN_IN},
    {"out", 3, TOKEN_OUT},
    {"gc", 2, TOKEN_GC},
    {"GC", 2, TOKEN_GC},
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(keywords[0]))

TokenKind keyword_lookup(const char *start, size_t length) {
    for (size_t i = 0; i < KEYWORD_COUNT; i++) {
        if (keywords[i].len == length && strncmp(keywords[i].word, start, length) == 0) {
            return keywords[i].kind;
        }
    }
    return TOKEN_IDENT;
}