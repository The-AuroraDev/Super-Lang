#ifndef SUPER_CGEN_STMT_H
#define SUPER_CGEN_STMT_H

#include "arena/arena.h"
#include "ast/ast.h"

void cgen_stmt(Arena *arena, AstNode *stmt);

#endif