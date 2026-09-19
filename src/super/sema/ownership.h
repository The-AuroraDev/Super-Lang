#ifndef SUPER_OWNERSHIP_H
#define SUPER_OWNERSHIP_H

#include "super/arena/arena.h"
#include "super/ast/ast.h"

bool ownership_analyze(Arena *arena, AstNode *file);

#endif