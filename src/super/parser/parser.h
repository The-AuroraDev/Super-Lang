#ifndef SUPER_PARSER_H
#define SUPER_PARSER_H

#include <stdbool.h>
#include "super/lexer/lexer.h"
#include "super/ast/ast.h"
#include "super/arena/arena.h"

typedef struct {
    Lexer *lexer;
    Token current;
    Token previous;
    Arena *arena;
    bool had_error;
    bool panic_mode;
} Parser;

Parser *parser_new(Arena *arena, Lexer *lexer);
AstNode *parser_parse(Parser *parser);
void parser_free(Parser *parser);

#endif