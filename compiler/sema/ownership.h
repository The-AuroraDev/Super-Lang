#ifndef SUPER_OWNERSHIP_H
#define SUPER_OWNERSHIP_H

#include "arena/arena.h"
#include "ast/ast.h"

bool ownership_analyze(Arena *arena, AstNode *file);

#endif