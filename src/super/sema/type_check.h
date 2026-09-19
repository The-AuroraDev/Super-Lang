#ifndef SUPER_TYPE_CHECK_H
#define SUPER_TYPE_CHECK_H

#include "super/arena/arena.h"
#include "super/ast/ast.h"

bool type_check(Arena *arena, AstNode *file);

#endif