#ifndef SUPER_TYPE_CHECK_H
#define SUPER_TYPE_CHECK_H

#include "arena/arena.h"
#include "ast/ast.h"

bool type_check(Arena *arena, AstNode *file);

#endif