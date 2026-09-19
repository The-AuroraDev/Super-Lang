#include "super/sema/sema.h"
#include "super/arena/arena.h"

Sema *sema_new(Arena *arena) {
    Sema *sema = arena_alloc_t(arena, Sema);
    sema->arena = arena;
    sema->current_fn = NULL;
    sema->loop_depth = 0;
    sema->had_error = false;
    return sema;
}

bool sema_analyze(Sema *sema, AstNode *file) {
    (void)sema;
    (void)file;
    return true;
}

void sema_free(Sema *sema) {
    (void)sema;
}