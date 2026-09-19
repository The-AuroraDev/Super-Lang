#ifndef SUPER_ARENA_H
#define SUPER_ARENA_H

#include <stddef.h>
#include <stdalign.h>

typedef struct Arena Arena;

Arena *arena_new(size_t initial_capacity);
void arena_free(Arena *arena);
void *arena_alloc(Arena *arena, size_t size, size_t align);
void *arena_calloc(Arena *arena, size_t count, size_t size, size_t align);
void arena_reset(Arena *arena);
size_t arena_used(Arena *arena);

#define arena_alloc_t(arena, T) ((T *)arena_alloc(arena, sizeof(T), alignof(T)))
#define arena_calloc_t(arena, T, n) ((T *)arena_calloc(arena, n, sizeof(T), alignof(T)))

typedef struct {
    Arena *arena;
    size_t checkpoint;
} ArenaCheckpoint;

static inline ArenaCheckpoint arena_checkpoint(Arena *arena) {
    return (ArenaCheckpoint){arena, arena_used(arena)};
}

static inline void arena_rewind(ArenaCheckpoint cp) {
    arena_reset(cp.arena);
}

#endif