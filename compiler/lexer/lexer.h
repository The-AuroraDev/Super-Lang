#ifndef SUPER_LEXER_H
#define SUPER_LEXER_H

#include "token.h"
#include "arena/arena.h"

typedef struct {
    const char *source;
    size_t length;
    size_t pos;
    size_t line;
    size_t column;
    size_t line_start;
    Arena *arena;
} Lexer;

Lexer *lexer_new(Arena *arena, const char *source, size_t length);
Token lexer_next(Lexer *lexer);
void lexer_free(Lexer *lexer);

#endif