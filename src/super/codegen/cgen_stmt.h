#ifndef SUPER_CGEN_STMT_H
#define SUPER_CGEN_STMT_H

#include "super/arena/arena.h"
#include "super/ast/ast.h"

void cgen_stmt(Arena *arena, AstNode *stmt);

#endif