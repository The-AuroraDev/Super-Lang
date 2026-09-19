#ifndef SUPER_CGEN_EXPR_H
#define SUPER_CGEN_EXPR_H

#include "super/arena/arena.h"
#include "super/ast/ast.h"

void cgen_expr(Arena *arena, AstNode *expr);

#endif