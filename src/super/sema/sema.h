#ifndef SUPER_SEMA_H
#define SUPER_SEMA_H

#include "super/ast/ast.h"
#include "super/arena/arena.h"

typedef struct {
    Arena *arena;
    AstNode *current_fn;
    int loop_depth;
    bool had_error;
} Sema;

Sema *sema_new(Arena *arena);
bool sema_analyze(Sema *sema, AstNode *file);
void sema_free(Sema *sema);

#endif