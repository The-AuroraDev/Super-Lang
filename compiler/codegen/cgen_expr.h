#ifndef SUPER_CGEN_EXPR_H
#define SUPER_CGEN_EXPR_H

#include "arena/arena.h"
#include "ast/ast.h"

void cgen_expr(Arena *arena, AstNode *expr);

#endif