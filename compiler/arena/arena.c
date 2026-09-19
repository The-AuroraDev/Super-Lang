#include "arena.h"
#include <stdlib.h>
#include <string.h>

#define ARENA_DEFAULT_CAPACITY (64 * 1024)
#define ARENA_MAX_ALIGN alignof(max_align_t)

struct Arena {
    char *base;
    char *ptr;
    char *end;
    Arena *next;
};

Arena *arena_new(size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = ARENA_DEFAULT_CAPACITY;
    Arena *arena = malloc(sizeof(Arena));
    if (!arena) return NULL;
    arena->base = malloc(initial_capacity);
    if (!arena->base) { free(arena); return NULL; }
    arena->ptr = arena->base;
    arena->end = arena->base + initial_capacity;
    arena->next = NULL;
    return arena;
}

void arena_free(Arena *arena) {
    while (arena) {
        Arena *next = arena->next;
        free(arena->base);
        free(arena);
        arena = next;
    }
}

static inline size_t align_up(size_t ptr, size_t align) {
    return (ptr + align - 1) & ~(align - 1);
}

void *arena_alloc(Arena *arena, size_t size, size_t align) {
    if (align > ARENA_MAX_ALIGN) align = ARENA_MAX_ALIGN;
    char *aligned_ptr = (char *)align_up((size_t)arena->ptr, align);
    size_t available = arena->end - aligned_ptr;
    if (available >= size) {
        arena->ptr = aligned_ptr + size;
        return memset(aligned_ptr, 0, size);
    }
    size_t new_capacity = (arena->end - arena->base) * 2;
    if (new_capacity < size + align) new_capacity = size + align + ARENA_DEFAULT_CAPACITY;
    Arena *new_arena = arena_new(new_capacity);
    if (!new_arena) return NULL;
    new_arena->next = arena->next;
    arena->next = new_arena;
    return arena_alloc(new_arena, size, align);
}

void *arena_calloc(Arena *arena, size_t count, size_t size, size_t align) {
    size_t total = count * size;
    void *ptr = arena_alloc(arena, total, align);
    return ptr;
}

void arena_reset(Arena *arena) {
    while (arena->next) {
        Arena *next = arena->next->next;
        free(arena->next->base);
        free(arena->next);
        arena->next = next;
    }
    arena->ptr = arena->base;
}

size_t arena_used(Arena *arena) {
    size_t used = arena->ptr - arena->base;
    for (Arena *a = arena->next; a; a = a->next) {
        used += a->ptr - a->base;
    }
    return used;
}